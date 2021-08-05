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

#ifndef __EXTRACTOR_SOURCE_HPP__
#define __EXTRACTOR_SOURCE_HPP__

#include "Source.hpp"
#include "AudioFormat.hpp"
#include "Buffer.hpp"
#include "Stream.hpp"
#include "FifoBuffer.hpp"
#include "ThreadBase.hpp"
#include <iostream>
#include <memory>
#include <mutex>

class ExtractorSource;
class ExtractorSource : public SourcePlugIn, public ThreadBase
{
protected:
  std::vector<AudioFormat> mSupportedFormats;
  std::shared_ptr<IStream> mpStream;
  AudioFormat mFormat;
  FifoBuffer mFifoBuffer;
  std::mutex mMutexStream;
  std::string mPath;
  std::string mParamKeyRoot;
  int mCallbackId;
  static inline int mInstanceCount = 0;
  int mChunkSize;

public:
  ExtractorSource();
  virtual ~ExtractorSource();
  virtual std::vector<AudioFormat> getSupportedAudioFormats(void){ return mSupportedFormats; };
  virtual bool isAvailableFormat(AudioFormat format);
  virtual bool open(std::string path);
  virtual void open(std::shared_ptr<IStream> pStream);
  virtual void close(void);
  virtual std::string getParameterKeyRoot(void){ return mParamKeyRoot; };

protected:
  virtual void readPrimitive(IAudioBuffer& buf);
  virtual void parse(ByteBuffer& inStreamBuf, IAudioBuffer& dstAudioBuf);
  virtual void process(void);

public:
  virtual std::string toString(void){ return "ExtractorSource"; };

  /* @desc initialize at loading the source plug-in shared object such as .so */
  virtual void onLoad(void){
    std::cout << "onLoad" << std::endl;
  }
  /* @desc uninitialize at unloading the source plug-in shared object such as .so */
  virtual void onUnload(void){
    std::cout << "onUnload" << std::endl;
  }
  /* @desc report the source plug-in's unique id
     @return unique plug-in id. may use uuid. */
  virtual std::string getId(void){
    return std::string("ExtractorSource");
  }
  /* @desc this is expected to use by strategy
     @return new instance of the class */
  virtual std::shared_ptr<IPlugIn> newInstance(void){
    return std::make_shared<ExtractorSource>();
  }
};

#endif /* __EXTRACTOR_SOURCE_HPP__ */