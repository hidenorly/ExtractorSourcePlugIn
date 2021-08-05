/*
  Copyright (C) 2021 hidenorly

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

       http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.
*/

#include "ExtractorSourcePlugIn.hpp"
#include "ExtractorPrimitive.hpp"
#include "ParameterManager.hpp"
#include "AudioFormatAdaptor.hpp"
#include <filesystem>
#include "Stream.hpp"

#define DEFAULT_CHUNK_SIZE 4096

ExtractorSource::ExtractorSource():SourcePlugIn(), mFormat( AudioFormat::ENCODING::COMPRESSED ), mChunkSize(DEFAULT_CHUNK_SIZE)
{
  for(int anEncoding = AudioFormat::ENCODING::PCM_8BIT; anEncoding < AudioFormat::ENCODING::COMPRESSED_UNKNOWN; anEncoding++){
    for( int aChannel = AudioFormat::CHANNEL::CHANNEL_MONO; aChannel < AudioFormat::CHANNEL::CHANNEL_UNKNOWN; aChannel++){
      mSupportedFormats.push_back( AudioFormat((AudioFormat::ENCODING)anEncoding, 48000, (AudioFormat::CHANNEL)aChannel) );
      mSupportedFormats.push_back( AudioFormat((AudioFormat::ENCODING)anEncoding, 96000, (AudioFormat::CHANNEL)aChannel) );
    }
  }

  mParamKeyRoot = "source.extractor_"+std::to_string(mInstanceCount++);

  ParameterManager* pParams = ParameterManager::getManager();
  ParameterManager::CALLBACK callback = [&](std::string key, std::string value){
    if( key ==  mParamKeyRoot+".path"){
      std::cout << "[ExtractorSource] path=" << value << std::endl;
      mPath = value;
      open(value);
    }
  };
  mCallbackId = pParams->registerCallback(mParamKeyRoot+".*", callback);

  mFifoBuffer.setAudioFormat( mFormat );
}

ExtractorSource::~ExtractorSource()
{
  stop();
  ParameterManager* pParams = ParameterManager::getManager();
  pParams->unregisterCallback(mCallbackId);
  mCallbackId = 0;
  close();
}

bool ExtractorSource::open(std::string path)
{
  bool result = std::filesystem::exists( path );
  if( result ){
    mPath = path;
    mMutexStream.lock();
    mpStream = std::make_shared<FileStream>(path);
    mMutexStream.unlock();
  }
  return result;
}

void ExtractorSource::open(std::shared_ptr<IStream> pStream)
{
  mMutexStream.lock();
  mpStream = pStream;
  mMutexStream.unlock();
}


void ExtractorSource::close(void)
{
  if( mpStream ){
    mMutexStream.lock();
    mpStream->close();
    mMutexStream.unlock();
    mpStream.reset();
  }
}

bool ExtractorSource::isAvailableFormat(AudioFormat format)
{
  return true;
}

void ExtractorSource::parse(ByteBuffer& inStreamBuf, IAudioBuffer& dstAudioBuf)
{
  ExtractorPrimitive::parse( inStreamBuf, dstAudioBuf);
}

void ExtractorSource::process(void)
{
  while( mbIsRunning && mpStream && !mpStream->isEndOfStream() ){
    CompressAudioBuffer buf( mFormat, mChunkSize );
    mFifoBuffer.setAudioFormat( mFormat );
    ByteBuffer rawBuf = buf.getRawBuffer();
    mMutexStream.lock();
    mpStream->read( rawBuf );
    mMutexStream.unlock();
    buf.setRawBuffer( rawBuf );
    mFifoBuffer.write( buf );
  }
}

void ExtractorSource::readPrimitive(IAudioBuffer& buf)
{
  if( !mbIsRunning && mpStream && !mpStream->isEndOfStream() ){
    run();
    CompressAudioBuffer inBuf( mFormat, mChunkSize );
    mFifoBuffer.read( inBuf );
    buf.setRawBuffer( inBuf.getRawBuffer() );
    parse( inBuf.getRawBuffer(), buf );
  } else {
    stop();
  }
}


extern "C"
{
void* getPlugInInstance(void)
{
  ExtractorSource* pInstance = new ExtractorSource();
  return reinterpret_cast<void*>(dynamic_cast<IPlugIn*>(pInstance));
}
};
