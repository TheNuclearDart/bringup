/**
  ******************************************************************************
  * File Name          : MJPEGDecoder.hpp
  ******************************************************************************
  * This file is generated by TouchGFX Generator 4.20.0. Please, do not edit!
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
#ifndef TOUCHGFX_MJPEGDECODER_HPP
#define TOUCHGFX_MJPEGDECODER_HPP

#include <touchgfx/hal/Types.hpp>
#include <touchgfx/hal/VideoController.hpp>

class MJPEGDecoder
{
public:
    virtual ~MJPEGDecoder()
    {
    }

    //Set video data for the decoder
    virtual void setVideoData(const uint8_t* movie, const uint32_t length) = 0;

    //Set video data for the decoder
    virtual void setVideoData(touchgfx::VideoDataReader& reader) = 0;

    /**
     * Check if MJPEGDecoder has a video.
     *
     * @return Returns true if the MJPEGDecoder has a video.
     */
    virtual bool hasVideo() = 0;

    //Increment position to next frame and decode and convert to RGB
    virtual bool decodeNextFrame(uint8_t* buffer, uint16_t width, uint16_t height, uint32_t stride) = 0;

    //Increment position to next frame and decode. Used with decodeFrame.
    virtual bool gotoNextFrame() = 0;

    //Decode part of the current frame, framebuffer is locked, area is drawn relative to frameBuffer
    virtual bool decodeFrame(const touchgfx::Rect& area, uint8_t* frameBuffer, uint32_t framebufferStride) = 0;

    //Decode thumbnail, assumes buffer stride is width
    virtual bool decodeThumbnail(uint32_t frameno, uint8_t* buffer, uint16_t width, uint16_t height) = 0;

    //Set current frame number
    virtual void gotoFrame(uint32_t frameno) = 0;

    //Get current frame number
    virtual uint32_t getCurrentFrameNumber() const = 0;

    //Get number of frames in video
    virtual uint32_t getNumberOfFrames() = 0;

    //Read video information
    virtual void getVideoInfo(touchgfx::VideoInformation* data) = 0;

    enum AVIErrors
    {
        AVI_NO_ERROR,
        AVI_NO_BUFFERS,
        AVI_NO_FILE,
        AVI_ERROR_NOT_RIFF,
        AVI_ERROR_AVI_HEADER_NOT_FOUND,
        AVI_ERROR_AVI_LIST_NOT_FOUND,
        AVI_ERROR_AVI_HDRL_NOT_FOUND,
        AVI_ERROR_AVI_AVIH_NOT_FOUND,
        AVI_ERROR_AVI_HEADER_TO_SHORT, //not full header provided
        AVI_ERROR_FILE_BUFFER_TO_SMALL,
        AVI_ERROR_MOVI_NOT_FOUND,
        AVI_ERROR_IDX1_NOT_FOUND,
        AVI_ERROR_FRAMENO_TO_HIGH,
        AVI_ERROR_EOF_REACHED
    };

    AVIErrors virtual getLastError() = 0;
};

#endif // TOUCHGFX_MJPEGDECODER_HPP
