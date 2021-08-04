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
#include "Buffer.hpp"
#include <cmath>
#include <iostream>

class ExtractorSource;
class ExtractorSource : public SourcePlugIn
{
protected:
  std::vector<AudioFormat> mSupportedFormats;

public:
  ExtractorSource();
  virtual ~ExtractorSource();
  virtual std::vector<AudioFormat> getSupportedAudioFormats(void){ return mSupportedFormats; };
  virtual bool isAvailableFormat(AudioFormat format);

protected:
  virtual void readPrimitive(IAudioBuffer& buf);

public:
  virtual std::string toString(void){ return "ExtractorSource"; };

  /* @desc initialize at loading the filter plug-in shared object such as .so */
  virtual void onLoad(void){
    std::cout << "onLoad" << std::endl;
  }
  /* @desc uninitialize at unloading the filter plug-in shared object such as .so */
  virtual void onUnload(void){
    std::cout << "onUnload" << std::endl;
  }
  /* @desc report your filter plug-in's unique id
     @return unique plug-in id. may use uuid. */
  virtual std::string getId(void){
    return std::string("ExtractorSource");
  }
  /* @desc this is expected to use by strategy
     @return new YourFilter()'s result */
  virtual std::shared_ptr<IPlugIn> newInstance(void){
    return std::make_shared<ExtractorSource>();
  }
};

#endif /* __EXTRACTOR_SOURCE_HPP__ */