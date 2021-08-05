# ExtractorSourcePlugIn

Extractor Source PlugIn for Audio Framework (https://github.com/hidenorly/audioframework.git)


# How to use?

Rapid way is to use fdk which is included in audioframework.

```
$ git clone https://github.com/hidenorly/audioframework.git
$ cd audioframework
$ make -j 4 afwshared; make -j 4 fdk
$ cd ..
$ cd ExtractorSourcePlugIn
$ make -j 4
$ cd ..
$ cd audioframework
$ bin/fdk_exec -u lib/source-plugin/libsource_extractor.dylib -p "source.extractor_0.path=inputfilename.mp3"
```

# External dependencies

* ffmpeg : https://github.com/FFmpeg/FFmpeg.git

## JFYI : how to build ffmpeg

### For Linux

```
$ sudo apt install libopus-dev libmp3lame-dev libfdk-aac-dev libvpx-dev yasm libass-dev libtheora-dev libvorbis-dev mercurial cmake build-essential libsnappy-dev opencl-dev liblzma-dev
$ mkdir ffmpeg-sdk
$ git clone https://github.com/FFmpeg/FFmpeg.git
$ cd FFmpeg
$ ./configure --prefix=../ffmpeg-sdk --enable-shared --enable-pthreads --enable-hardcoded-tables --cc=clang --host-cflags= --host-ldflags= --enable-ffplay --enable-libmp3lame --enable-libopus --enable-libsnappy --enable-libtheora --enable-libvorbis --enable-libvpx  --enable-lzma --enable-opencl
$ make -j 4
$ make install
```

### For MacOS

```
$ mkdir ffmpeg-sdk
$ git clone https://github.com/FFmpeg/FFmpeg.git
$ cd FFmpeg
$ ./configure --prefix=../ffmpeg-sdk --enable-shared --enable-pthreads --enable-hardcoded-tables --cc=clang --host-cflags= --host-ldflags= --enable-ffplay --enable-libmp3lame --enable-libopus --enable-libsnappy --enable-libtheora --enable-libvorbis --enable-libvpx  --enable-lzma --enable-opencl --enable-videotoolbox
$ make -j 4
$ make install
```