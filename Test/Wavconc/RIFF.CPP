/*==========================================================================

   riff.cpp  -  Don Cross, April 1993.

   Contains code for reading/writing RIFF files, including .WAV files!

   See also:
     ..\include\riff.h
     ..\include\ddc.h

   Revision history:

1993 April 12 [Don Cross]
   Started writing this code.

1994 October 6 [Don Cross]
   Added WriteData, ReadData.
   Added const modifiers to some function parameters.

1997 September 14 [Don Cross]
     Fixed a bug in WaveFile::Close.  It was calling Backpatch
   and getting confused when the the file had been opened for read.
   (Backpatch returns an error in that case, which prevented
   WaveFile::Close from calling RiffFile::Close.)

==========================================================================*/

// Standard includes
#include "stdafx.h"
#include <io.h>
#include <stdio.h>
#include <stdlib.h>
#include <String.h>

// DDCLIB includes
#include "riff.h"

UINT32 FourCC (const char *ChunkName) {
   UINT32 retbuf = 0x20202020;   // four spaces (padding)
   char *p = ((char *)&retbuf);

   // Remember, this is Intel format!
   // The first character goes in the LSB
   for(int i = 0; i < 4 && ChunkName[i]; i++) {
     *p++ = ChunkName[i];
   }
   return retbuf;
}

//----------------------------------------------------------------------

RiffFile::RiffFile() {
  file  = NULL;
  fmode = RFM_UNKNOWN;

  riff_header.ckID   = FourCC("RIFF");
  riff_header.ckSize = 0;
}

RiffFile::~RiffFile() {
  if(fmode != RFM_UNKNOWN) {
    Close();
  }
}

DDCRET RiffFile::Open(const char *Filename, RiffFileMode NewMode) {
  DDCRET retcode = DDC_SUCCESS;
  if(fmode != RFM_UNKNOWN) {
    retcode = Close();
  }
  if(retcode == DDC_SUCCESS) {
    switch(NewMode) {
    case RFM_WRITE:
      file = fopen(Filename, "wb");
      if(file) {
        // Write the RIFF header...
        // We will have to come back later and patch it!
        if(fwrite(&riff_header, sizeof(riff_header), 1, file) != 1) {
          fclose(file);
          _unlink(Filename);
          fmode = RFM_UNKNOWN;
          file = 0;
        } else {
          fmode = RFM_WRITE;
        }
      } else {
        fmode = RFM_UNKNOWN;
        retcode = DDC_FILE_ERROR;
      }
      break;
    case RFM_READ:
      file = fopen(Filename, "rb");
      if(file) {
        // Try to read the RIFF header...
        if(fread(&riff_header, sizeof(riff_header), 1, file) != 1) {
          fclose(file);
          fmode = RFM_UNKNOWN;
          file  = NULL;
        } else {
          fmode = RFM_READ;
        }
      } else {
        fmode   = RFM_UNKNOWN;
        retcode = DDC_FILE_ERROR;
      }
      break;
    default:
      retcode = DDC_INVALID_CALL;
      break;
    }
  }
  return retcode;
}

DDCRET RiffFile::Write(const void *Data, UINT NumBytes) {
  if(fmode != RFM_WRITE) {
    return DDC_INVALID_CALL;
  }
  if(fwrite(Data, NumBytes, 1, file) != 1) {
    return DDC_FILE_ERROR;
  }
  riff_header.ckSize += NumBytes;
  return DDC_SUCCESS;
}

DDCRET RiffFile::Close() {
  DDCRET retcode = DDC_SUCCESS;
  switch(fmode) {
  case RFM_WRITE:
    if(fflush(file) || fseek(file,0,SEEK_SET)
    || (fwrite(&riff_header, sizeof(riff_header), 1, file) != 1)
    || fclose(file)) {
      retcode = DDC_FILE_ERROR;
    }
    break;

  case RFM_READ:
    fclose(file);
    break;
  }
  file  = NULL;
  fmode = RFM_UNKNOWN;
  return retcode;
}

long RiffFile::CurrentFilePosition() const {
  return ftell(file);
}

DDCRET RiffFile::Seek(long offset) {
  fflush(file);
  DDCRET rc;
  if(fseek(file, offset, SEEK_SET)) {
    rc = DDC_FILE_ERROR;
  } else {
    rc = DDC_SUCCESS;
  }
  return rc;
}

DDCRET RiffFile::Backpatch(long        FileOffset
                          ,const void *Data
                          ,UINT        NumBytes)
{
  if(!file) {
    return DDC_INVALID_CALL;
  }
  if(fflush(file) || fseek(file, FileOffset, SEEK_SET)) {
    return DDC_FILE_ERROR;
  }
  return Write(Data, NumBytes);
}

DDCRET RiffFile::Expect(const void *Data, UINT NumBytes) {
  char *p = (char*)Data;
  while(NumBytes--) {
    if(fgetc(file) != *p++) {
      return DDC_FILE_ERROR;
    }
  }
  return DDC_SUCCESS;
}

DDCRET RiffFile::Read(void *Data, UINT NumBytes) {
  DDCRET retcode = DDC_SUCCESS;
  if(fread(Data,NumBytes,1,file) != 1) {
    retcode = DDC_FILE_ERROR;
  }
  return retcode;
}

//-----------------------------------------------------------------------

WaveFile::WaveFile() {
  pcm_data.ckID = FourCC("data");
  pcm_data.ckSize = 0;
  num_samples = 0;
}

DDCRET WaveFile::OpenForRead(const char *Filename) {
   // Verify filename parameter as best we can...
  if(!Filename) {
    return DDC_INVALID_CALL;
  }

  DDCRET retcode = Open(Filename, RFM_READ);
  if(retcode == DDC_SUCCESS) {
    retcode = Expect("WAVE", 4);

    if(retcode == DDC_SUCCESS) {
      retcode = Read(&wave_format, sizeof(wave_format));

      if(retcode == DDC_SUCCESS && !wave_format.VerifyValidity()) {
        // This isn't standard PCM, so we don't know what it is!
        retcode = DDC_FILE_ERROR;
      }

      if(retcode == DDC_SUCCESS) {
        pcm_data_offset = CurrentFilePosition();

        // Figure out number of samples from
        // file size, current file position, and
        // WAVE header.

        retcode = Read(&pcm_data, sizeof(pcm_data));
        num_samples = _filelength(_fileno(file)) - CurrentFilePosition();
        num_samples /= NumChannels();
        num_samples /= (BitsPerSample() / 8);
      }
    }
  }
  return retcode;
}

DDCRET WaveFile::OpenForWrite( const char  *Filename
                              ,UINT32       SamplingRate
                              ,UINT16       BitsPerSample
                              ,UINT16       NumChannels)
{
   // Verify parameters...
  if(!Filename || (BitsPerSample != 8 && BitsPerSample != 16)
   || NumChannels < 1 || NumChannels > 2) {
    return DDC_INVALID_CALL;
  }

  wave_format.data.Config(SamplingRate, BitsPerSample, NumChannels);

  DDCRET retcode = Open(Filename, RFM_WRITE);

  if(retcode == DDC_SUCCESS) {
    retcode = Write("WAVE", 4);

    if(retcode == DDC_SUCCESS) {
      retcode = Write(&wave_format, sizeof(wave_format));
      if(retcode == DDC_SUCCESS) {
        pcm_data_offset = CurrentFilePosition();
        retcode = Write ( &pcm_data, sizeof(pcm_data));
      }
    }
  }
  return retcode;
}

DDCRET WaveFile::Close() {
  DDCRET rc = DDC_SUCCESS;

  if(fmode == RFM_WRITE) {
    rc = Backpatch(pcm_data_offset, &pcm_data, sizeof(pcm_data));
  }
  if(rc == DDC_SUCCESS) {
    rc = RiffFile::Close();
  }
  return rc;
}

DDCRET WaveFile::WriteSample(const INT16 Sample[MAX_WAVE_CHANNELS]) {
  DDCRET retcode = DDC_SUCCESS;
  switch(wave_format.data.nChannels) {
  case 1:
    switch(wave_format.data.nBitsPerSample) {
    case 8:
      pcm_data.ckSize += 1;
      retcode = Write(&Sample[0], 1);
      break;

    case 16:
      pcm_data.ckSize += 2;
      retcode = Write(&Sample[0], 2);
      break;

    default:
      retcode = DDC_INVALID_CALL;
      break;
    } // switch(wave_format.data.nBitsPerSample)
    break;
  case 2:
    switch(wave_format.data.nBitsPerSample) {
    case 8:
      retcode = Write(&Sample[0], 1);
      if(retcode == DDC_SUCCESS) {
        retcode = Write(&Sample[1], 1);
        if(retcode == DDC_SUCCESS) {
          pcm_data.ckSize += 2;
        }
      }
      break;
    case 16:
      retcode = Write(&Sample[0], 2);
      if(retcode == DDC_SUCCESS) {
        retcode = Write(&Sample[1], 2);
        if(retcode == DDC_SUCCESS) {
          pcm_data.ckSize += 4;
        }
      }
      break;
    default:
      retcode = DDC_INVALID_CALL;
    } // switch(wave_format.data.nBitsPerSample)
    break;
  default:
    retcode = DDC_INVALID_CALL;
    break;
  } // switch(wave_format.data.nChannels)
  return retcode;
}

DDCRET WaveFile::WriteMonoSample(INT16 SampleData) {
  switch(wave_format.data.nBitsPerSample) {
  case 8:
    pcm_data.ckSize += 1;
    return Write(&SampleData, 1);
  case 16:
    pcm_data.ckSize += 2;
    return Write(&SampleData, 2);
  }
  return DDC_INVALID_CALL;
}

DDCRET WaveFile::WriteStereoSample(INT16 LeftSample, INT16 RightSample) {
  DDCRET retcode = DDC_SUCCESS;

  switch(wave_format.data.nBitsPerSample) {
  case 8:
    retcode = Write(&LeftSample, 1);
    if(retcode == DDC_SUCCESS) {
      retcode = Write(&RightSample, 1);
      if(retcode == DDC_SUCCESS) {
        pcm_data.ckSize += 2;
      }
    }
    break;
  case 16:
    retcode = Write(&LeftSample, 2);
    if(retcode == DDC_SUCCESS) {
      retcode = Write(&RightSample, 2);
      if(retcode == DDC_SUCCESS) {
        pcm_data.ckSize += 4;
      }
    }
    break;
  default:
    retcode = DDC_INVALID_CALL;
    break;
  }
  return retcode;
}

DDCRET WaveFile::ReadSample(INT16 Sample [MAX_WAVE_CHANNELS]) {
  DDCRET retcode = DDC_SUCCESS;

  switch(wave_format.data.nChannels) {
  case 1:
    switch(wave_format.data.nBitsPerSample) {
    case 8:
      unsigned char x;
      retcode = Read(&x, 1);
      Sample[0] = INT16(x);
      break;

    case 16:
      retcode = Read(&Sample[0], 2);
      break;

    default:
      retcode = DDC_INVALID_CALL;
      break;
    } // switch(wave_format.data.nBitsPerSample)
    break;
  case 2:
    switch(wave_format.data.nBitsPerSample) {
    case 8:
      unsigned char  x[2];
      retcode = Read(x, 2);
      Sample[0] = INT16(x[0]);
      Sample[1] = INT16(x[1]);
      break;
    case 16:
      retcode = Read(Sample, 4);
      break;
    default:
      retcode = DDC_INVALID_CALL;
    } // switch(wave_format.data.nBitsPerSample)
    break;
  default:
    retcode = DDC_INVALID_CALL;
    break;
  } // switch(wave_format.data.nChannels)
  return retcode;
}

DDCRET WaveFile::ReadSamples(INT32 num, WaveFileSample sarray[]) {
  DDCRET retcode = DDC_SUCCESS;
  INT32 i;

  switch(wave_format.data.nChannels) {
  case 1:
    switch(wave_format.data.nBitsPerSample) {
    case 8:
      for(i = 0; i < num && retcode == DDC_SUCCESS; i++) {
        unsigned char x;
        retcode = Read(&x, 1);
        sarray[i].chan[0] = INT16(x);
      }
      break;
    case 16:
      for(i = 0; i < num && retcode == DDC_SUCCESS; i++) {
        retcode = Read(&sarray[i].chan[0], 2);
      }
      break;
    default:
      retcode = DDC_INVALID_CALL;
      break;
    } // switch(wave_format.data.nBitsPerSample)
    break;
  case 2:
    switch(wave_format.data.nBitsPerSample) {
    case 8:
      for(i = 0; i < num && retcode == DDC_SUCCESS; i++) {
        unsigned char x[2];
        retcode = Read(x, 2);
        sarray[i].chan[0] = INT16 ( x[0]);
        sarray[i].chan[1] = INT16 ( x[1]);
      }
      break;
    case 16:
      retcode = Read(sarray, 4 * num);
      break;
    default:
      retcode = DDC_INVALID_CALL;
      break;
    } // switch(wave_format.data.nBitsPerSample)
    break;
  default:
    retcode = DDC_INVALID_CALL;
    break;
  } // switch(wave_format.data.nChannels)
  return retcode;
}


DDCRET WaveFile::ReadMonoSample(INT16 *Sample) {
  DDCRET retcode = DDC_SUCCESS;
  switch( wave_format.data.nBitsPerSample) {
  case 8:
    unsigned char x;
    retcode = Read(&x, 1);
    *Sample = INT16(x);
    break;
  case 16:
    retcode = Read(Sample, 2);
    break;
  default:
    retcode = DDC_INVALID_CALL;
    break;
  }
  return retcode;
}


DDCRET WaveFile::ReadStereoSample(INT16 *L, INT16 *R) {
  DDCRET retcode = DDC_SUCCESS;
  UINT8          x[2];
  INT16          y[2];

  switch(wave_format.data.nBitsPerSample) {
  case 8:
    retcode = Read(x, 2);
    *L = INT16(x[0]);
    *R = INT16(x[1]);
    break;
  case 16:
    retcode = Read(y, 4);
    *L = INT16(y[0]);
    *R = INT16(y[1]);
    break;
  default:
    retcode = DDC_INVALID_CALL;
    break;
  }
  return retcode;
}

DDCRET WaveFile::SeekToSample(ULONG SampleIndex) {
  if(SampleIndex >= NumSamples()) {
    return DDC_INVALID_CALL;
  }
  UINT SampleSize = (BitsPerSample() + 7) / 8;
  DDCRET rc = Seek(pcm_data_offset + sizeof(pcm_data)
                 + SampleSize * NumChannels() * SampleIndex);
  return rc;
}

UINT32 WaveFile::SamplingRate() const {
  return wave_format.data.nSamplesPerSec;
}

UINT16 WaveFile::BitsPerSample() const {
  return wave_format.data.nBitsPerSample;
}

UINT16 WaveFile::NumChannels() const {
  return wave_format.data.nChannels;
}

UINT32 WaveFile::NumSamples() const {
  return num_samples;
}

DDCRET WaveFile::WriteData(const INT16 *data, UINT32 numData) {
  UINT32 extraBytes = numData * sizeof(INT16);
  pcm_data.ckSize += extraBytes;
  return RiffFile::Write(data, extraBytes);
}

DDCRET WaveFile::WriteData(const UINT8 *data, UINT32 numData) {
  pcm_data.ckSize += numData;
  return RiffFile::Write(data, numData);
}

DDCRET WaveFile::ReadData(INT16 *data, UINT32 numData) {
  return RiffFile::Read(data, numData * sizeof(INT16));
}

DDCRET WaveFile::ReadData(UINT8 *data, UINT32 numData) {
  return RiffFile::Read(data, numData);
}

/*--- end of file riff.cpp ---*/
