//
// Copyright (c) Microsoft. All rights reserved.
// See https://aka.ms/csspeech/license201809 for the full license information.
//
// speechapi_cxx_audio_stream.h: Public API declarations for AudioInputStream / AudioOutputStream and related C++ classes
//

#pragma once
#include <functional>
#include <future>
#include <memory>
#include <vector>
#include <string>
#include <spxdebug.h>
#include <speechapi_cxx_common.h>
#include <speechapi_cxx_smart_handle.h>
#include <speechapi_cxx_audio_stream_format.h>
#include <speechapi_c_audio_stream.h>


namespace Microsoft {
namespace CognitiveServices {
namespace Speech {
namespace Audio {


class PullAudioInputStreamCallback;
class PushAudioInputStream;
class PullAudioInputStream;
class PushAudioOutputStreamCallback;
class PushAudioOutputStream;
class PullAudioOutputStream;


/// <summary>
/// Represents audio input stream used for custom audio input configurations.
/// </summary>
class AudioInputStream
{
public:

#if defined(BINDING_OBJECTIVE_C) || !defined(SWIG)
    using ReadCallbackFunction_Type = ::std::function<int(uint8_t*, uint32_t)>;
    using CloseCallbackFunction_Type = ::std::function<void()>;
#endif // defined(BINDING_OBJECTIVE_C) || !defined(SWIG)

    /// <summary>
    /// Internal operator used to get underlying handle value.
    /// </summary>
    explicit operator SPXAUDIOSTREAMHANDLE() const { return m_haudioStream.get(); }

    /// <summary>
    /// Creates a memory backed PushAudioInputStream using the default format (16Khz 16bit mono PCM).
    /// </summary>
    /// <returns>A shared pointer to PushAudioInputStream</returns>
    static std::shared_ptr<PushAudioInputStream> CreatePushStream();

    /// <summary>
    /// Creates a memory backed PushAudioInputStream with the specified audio format.
    /// </summary>
    /// <param name="format">Audio stream format.</param>
    /// <returns>A shared pointer to PushAudioInputStream</returns>
    static std::shared_ptr<PushAudioInputStream> CreatePushStream(std::shared_ptr<AudioStreamFormat> format);

#if defined(BINDING_OBJECTIVE_C) || !defined(SWIG)
    /// <summary>
    /// Creates a PullAudioInputStream that delegates to the specified callback functions for Read() and Close() methods, using the default format (16Khz 16bit mono PCM).
    /// </summary>
    /// <param name="pvContext">Context pointer to use when invoking the callbacks.</param>
    /// <param name="readCallback">Read callback.</param>
    /// <param name="closeCallback">Close callback.</param>
    /// <returns>A shared pointer to PullAudioInputStream</returns>
    static std::shared_ptr<PullAudioInputStream> CreatePullStream(void* pvContext, CUSTOM_AUDIO_PULL_STREAM_READ_CALLBACK readCallback, CUSTOM_AUDIO_PULL_STREAM_CLOSE_CALLBACK closeCallback = nullptr);

    /// <summary>
    /// Creates a PullAudioInputStream that delegates to the specified callback functions for Read() and Close() methods, using the default format (16Khz 16bit mono PCM).
    /// </summary>
    /// <param name="readCallback">Read callback.</param>
    /// <param name="closeCallback">Close callback.</param>
    /// <returns>A shared pointer to PullAudioInputStream</returns>
    static std::shared_ptr<PullAudioInputStream> CreatePullStream(ReadCallbackFunction_Type readCallback, CloseCallbackFunction_Type closeCallback = nullptr);
#endif // defined(BINDING_OBJECTIVE_C) || !defined(SWIG)

    /// <summary>
    /// Creates a PullAudioInputStream that delegates to the specified callback interface for the Read() and Close() methods, using the default format (16Khz 16bit mono PCM).
    /// </summary>
    /// <param name="callback">Shared pointer to PullAudioInputStreamCallback instance.</param>
    /// <returns>A shared pointer to PullAudioInputStream</returns>
    static std::shared_ptr<PullAudioInputStream> CreatePullStream(std::shared_ptr<PullAudioInputStreamCallback> callback);

#if defined(BINDING_OBJECTIVE_C) || !defined(SWIG)
    /// <summary>
    /// Creates a PullAudioInputStream that delegates to the specified callback functions for Read() and Close() methods.
    /// </summary>
    /// <param name="format">Audio stream format.</param>
    /// <param name="pvContext">Context pointer to use when invoking the callbacks.</param>
    /// <param name="readCallback">Read callback.</param>
    /// <param name="closeCallback">Close callback.</param>
    /// <returns>A shared pointer to PullAudioInputStream</returns>
    static std::shared_ptr<PullAudioInputStream> CreatePullStream(std::shared_ptr<AudioStreamFormat> format, void* pvContext, CUSTOM_AUDIO_PULL_STREAM_READ_CALLBACK readCallback, CUSTOM_AUDIO_PULL_STREAM_CLOSE_CALLBACK closeCallback = nullptr);

    /// <summary>
    /// Creates a PullAudioInputStream that delegates to the specified callback functions for Read() and Close() methods.
    /// </summary>
    /// <param name="format">Audio stream format.</param>
    /// <param name="readCallback">Read callback.</param>
    /// <param name="closeCallback">Close callback.</param>
    /// <returns>A shared pointer to PullAudioInputStream</returns>
    static std::shared_ptr<PullAudioInputStream> CreatePullStream(std::shared_ptr<AudioStreamFormat> format, ReadCallbackFunction_Type readCallback, CloseCallbackFunction_Type closeCallback = nullptr);
#endif // defined(BINDING_OBJECTIVE_C) || !defined(SWIG)

    /// <summary>
    /// Creates a PullAudioInputStream that delegates to the specified callback interface for the Read() and Close() methods, using the specified format.
    /// </summary>
    /// <param name="format">Audio stream format.</param>
    /// <param name="callback">Shared pointer to PullAudioInputStreamCallback instance.</param>
    /// <returns>A shared pointer to PullAudioInputStream</returns>
    static std::shared_ptr<PullAudioInputStream> CreatePullStream(std::shared_ptr<AudioStreamFormat> format, std::shared_ptr<PullAudioInputStreamCallback> callback);

protected:

    /*! \cond PROTECTED */

    /// <summary>
    /// Internal constructor. Creates a new instance using the provided handle.
    /// </summary>
    explicit AudioInputStream(SPXAUDIOSTREAMHANDLE haudioStream) : m_haudioStream(haudioStream) { }

    /// <summary>
    /// Destructor, does nothing.
    /// </summary>
    virtual ~AudioInputStream() {}

    /// <summary>
    /// Internal helper method to get the default format if the specified format is nullptr.
    /// </summary>
    static std::shared_ptr<AudioStreamFormat> UseDefaultFormatIfNull(std::shared_ptr<AudioStreamFormat> format) { return format != nullptr ? format : AudioStreamFormat::GetDefaultInputFormat(); }

    /// <summary>
    /// Internal helper method to get the audio stream format handle.
    /// </summary>
    static SPXAUDIOSTREAMFORMATHANDLE GetFormatHandle(std::shared_ptr<AudioStreamFormat> format) { return (SPXAUDIOSTREAMFORMATHANDLE)(*format.get()); }
    
    /// <summary>
    /// Internal member variable that holds the smart handle.
    /// </summary>
    SmartHandle<SPXAUDIOSTREAMHANDLE, &audio_stream_release> m_haudioStream;

    /*! \endcond */

private:

    DISABLE_COPY_AND_MOVE(AudioInputStream);
};


/// <summary>
/// Represents memory backed push audio input stream used for custom audio input configurations.
/// </summary>
class PushAudioInputStream : public AudioInputStream
{
public:

    /// <summary>
    /// Destructor; closes the underlying stream if not already closed.
    /// </summary>
    virtual ~PushAudioInputStream()
    {
        if (audio_stream_is_handle_valid(m_haudioStream))
        {
            CloseStream();
        }
    }

    /// <summary>
    /// Creates a memory backed PushAudioInputStream using the default format (16Khz 16bit mono PCM).
    /// </summary>
    /// <returns>A shared pointer to PushAudioInputStream</returns>
    static std::shared_ptr<PushAudioInputStream> Create()
    {
        return Create(nullptr);
    }

    /// <summary>
    /// Creates a memory backed PushAudioInputStream with the specified audio format.
    /// </summary>
    /// <param name="format">Audio stream format.</param>
    /// <returns>A shared pointer to PushAudioInputStream</returns>
    static std::shared_ptr<PushAudioInputStream> Create(std::shared_ptr<AudioStreamFormat> format)
    {
        format = UseDefaultFormatIfNull(format);

        SPXAUDIOSTREAMHANDLE haudioStream = SPXHANDLE_INVALID;
        SPX_THROW_ON_FAIL(audio_stream_create_push_audio_input_stream(&haudioStream, GetFormatHandle(format)));

        auto stream = new PushAudioInputStream(haudioStream);
        return std::shared_ptr<PushAudioInputStream>(stream);
    }

    /// <summary>
    /// Writes the audio data specified by making an internal copy of the data.
    /// Note: The dataBuffer should not contain any audio header.
    /// </summary>
    /// <param name="dataBuffer">The pointer to the audio buffer of which this function will make a copy.</param>
    /// <param name="size">The size of the buffer.</param>
    void Write(uint8_t* dataBuffer, uint32_t size) { SPX_THROW_ON_FAIL(push_audio_input_stream_write(m_haudioStream, dataBuffer, size)); }

    /// <summary>
    /// Closes the stream.
    /// </summary>
    void Close() { SPX_THROW_ON_FAIL(CloseStream()); }


protected:

    /*! \cond PROTECTED */

    /// <summary>
    /// Internal constructor. Creates a new instance using the provided handle.
    /// </summary>
    explicit PushAudioInputStream(SPXAUDIOSTREAMHANDLE haudioStream) : AudioInputStream(haudioStream) { }

    /*! \endcond */

private:

    DISABLE_COPY_AND_MOVE(PushAudioInputStream);

    SPXHR CloseStream() { return push_audio_input_stream_close(m_haudioStream); }
};


/// <summary>
/// An interface that defines callback methods for an audio input stream.
/// </summary>
/// <remarks>
/// Derive from this class and implement its function to provide your own
/// data as an audio input stream.
/// <remarks>
class PullAudioInputStreamCallback
{
public:

    /// <summary>
    /// Destructor, does nothing.
    /// </summary>
    virtual ~PullAudioInputStreamCallback() {}

    /// <summary>
    /// This function is called to synchronously get data from the audio stream.
    /// Note: The dataBuffer returned by Read() should not contain any audio header.
    /// </summary>
    /// <param name="dataBuffer">The pointer to the buffer to which to copy the audio data.</param>
    /// <param name="size">The size of the buffer.</param>
    /// <returns>The number of bytes copied into the buffer, or zero to indicate end of stream</returns>
    virtual int Read(uint8_t* dataBuffer, uint32_t size) = 0;

    /// <summary>
    /// This function is called to close the audio stream.
    /// </summary>
    virtual void Close() = 0;

protected:

    /*! \cond PROTECTED */

    /// <summary>
    /// Constructor, does nothing.
    /// </summary>
    PullAudioInputStreamCallback() {};

    /*! \endcond */

private:

    DISABLE_COPY_AND_MOVE(PullAudioInputStreamCallback);
};


/// <summary>
/// Pull audio input stream class.
/// </summary>
class PullAudioInputStream : public AudioInputStream
{
public:

#if defined(BINDING_OBJECTIVE_C) || !defined(SWIG)
    /// <summary>
    /// Creates a PullAudioInputStream utilizing the specified Read() and Close() "C" callback functions pointers
    /// Note: The dataBuffer returned by Read() should not contain any audio header.
    /// </summary>
    /// <param name="pvContext">Context pointer to use when invoking the callbacks.</param>
    /// <param name="readCallback">Read callback.</param>
    /// <param name="closeCallback">Close callback.</param>
    /// <returns>A shared pointer to PullAudioInputStream</returns>
    static std::shared_ptr<PullAudioInputStream> Create(void* pvContext, CUSTOM_AUDIO_PULL_STREAM_READ_CALLBACK readCallback, CUSTOM_AUDIO_PULL_STREAM_CLOSE_CALLBACK closeCallback = nullptr)
    {
        return Create(nullptr, pvContext, readCallback, closeCallback);
    }

    /// <summary>
    /// Creates a PullAudioInputStream utilizing the specified Read() and Close() callback functions.
    /// Note: The dataBuffer returned by Read() should not contain any audio header.
    /// </summary>
    /// <param name="readCallback">Read callback.</param>
    /// <param name="closeCallback">Close callback.</param>
    /// <returns>A shared pointer to PullAudioInputStream</returns>
    static std::shared_ptr<PullAudioInputStream> Create(ReadCallbackFunction_Type readCallback, CloseCallbackFunction_Type closeCallback = nullptr)
    {
        return Create(nullptr, readCallback, closeCallback);
    }
#endif // defined(BINDING_OBJECTIVE_C) || !defined(SWIG)

    /// <summary>
    /// Creates a PullAudioInputStream utilizing the specified Close() callback function.
    /// </summary>
    /// <param name="callback">Shared pointer to PullAudioInputStreamCallback instance.</param>
    /// <returns>A shared pointer to PullAudioInputStream</returns>
    static std::shared_ptr<PullAudioInputStream> Create(std::shared_ptr<PullAudioInputStreamCallback> callback)
    {
        return Create(nullptr, callback);
    }

#if defined(BINDING_OBJECTIVE_C) || !defined(SWIG)
    /// <summary>
    /// Creates a PullAudioInputStream utilizing the specified Read() and Close() "C" callback functions pointers
    /// Note: The dataBuffer returned by Read() should not contain any audio header.
    /// </summary>
    /// <param name="format">Audio stream format.</param>
    /// <param name="pvContext">Context pointer to use when invoking the callbacks.</param>
    /// <param name="readCallback">Read callback.</param>
    /// <param name="closeCallback">Close callback.</param>
    /// <returns>A shared pointer to PullAudioInputStream</returns>
    static std::shared_ptr<PullAudioInputStream> Create(std::shared_ptr<AudioStreamFormat> format, void* pvContext, CUSTOM_AUDIO_PULL_STREAM_READ_CALLBACK readCallback, CUSTOM_AUDIO_PULL_STREAM_CLOSE_CALLBACK closeCallback = nullptr)
    {
        return Create(format,
            [=](uint8_t* buffer, uint32_t size) -> int { return readCallback(pvContext, buffer, size); },
            [=]() { if (closeCallback != nullptr) { closeCallback(pvContext); } });
    }

    /// <summary>
    /// Creates a PullAudioInputStream utilizing the specified Read() and Close() callback functions.
    /// Note: The dataBuffer returned by Read() should not contain any audio header.
    /// </summary>
    /// <param name="format">Audio stream format.</param>
    /// <param name="readCallback">Read callback.</param>
    /// <param name="closeCallback">Close callback.</param>
    /// <returns>A shared pointer to PullAudioInputStream</returns>
    static std::shared_ptr<PullAudioInputStream> Create(std::shared_ptr<AudioStreamFormat> format, ReadCallbackFunction_Type readCallback, CloseCallbackFunction_Type closeCallback = nullptr)
    {
        auto wrapper = std::make_shared<FunctionCallbackWrapper>(readCallback, closeCallback);
        return Create(format, wrapper);
    }
#endif // defined(BINDING_OBJECTIVE_C) || !defined(SWIG)

    /// <summary>
    /// Creates a PullAudioInputStream utilizing the specified Read() and Close() callback functions.
    /// </summary>
    /// <param name="format">Audio stream format.</param>
    /// <param name="callback">Shared pointer to PullAudioInputStreamCallback instance.</param>
    /// <returns>A shared pointer to PullAudioInputStream</returns>
    static std::shared_ptr<PullAudioInputStream> Create(std::shared_ptr<AudioStreamFormat> format, std::shared_ptr<PullAudioInputStreamCallback> callback)
    {
        format = UseDefaultFormatIfNull(format);

        SPXAUDIOSTREAMHANDLE haudioStream = SPXHANDLE_INVALID;
        SPX_THROW_ON_FAIL(audio_stream_create_pull_audio_input_stream(&haudioStream, GetFormatHandle(format)));

        auto stream = new PullAudioInputStream(haudioStream);
        SPX_THROW_ON_FAIL(pull_audio_input_stream_set_callbacks(haudioStream, stream, ReadCallbackWrapper, CloseCallbackWrapper));
        stream->m_callback = callback;

        return std::shared_ptr<PullAudioInputStream>(stream);
    }

protected:

    /*! \cond PROTECTED */

    /// <summary>
    /// Internal constructor. Creates a new instance using the provided handle.
    /// </summary>
    explicit PullAudioInputStream(SPXAUDIOSTREAMHANDLE haudioStream) : AudioInputStream(haudioStream) { }

#if defined(BINDING_OBJECTIVE_C) || !defined(SWIG)
    class FunctionCallbackWrapper : public PullAudioInputStreamCallback
    {
    public:

        FunctionCallbackWrapper(ReadCallbackFunction_Type readCallback, CloseCallbackFunction_Type closeCallback) :
            m_readCallback(readCallback),
            m_closeCallback(closeCallback)
        {
        };

        /// Note: The dataBuffer returned by Read() should not contain any audio header.
        int Read(uint8_t* dataBuffer, uint32_t size) override { return m_readCallback(dataBuffer, size); }
        void Close() override { if (m_closeCallback != nullptr) m_closeCallback(); };

    private:

        DISABLE_COPY_AND_MOVE(FunctionCallbackWrapper);

        ReadCallbackFunction_Type m_readCallback;
        CloseCallbackFunction_Type m_closeCallback;
    };
#endif // defined(BINDING_OBJECTIVE_C) || !defined(SWIG)

    /*! \endcond */

private:

    DISABLE_COPY_AND_MOVE(PullAudioInputStream);

    static int ReadCallbackWrapper(void* pvContext, uint8_t* dataBuffer, uint32_t size)
    {
        PullAudioInputStream* ptr = (PullAudioInputStream*)pvContext;
        return ptr->m_callback->Read(dataBuffer, size);
    }

    static void CloseCallbackWrapper(void* pvContext)
    {
        SPX_DBG_TRACE_SCOPE(__FUNCTION__, __FUNCTION__);
        PullAudioInputStream* ptr = (PullAudioInputStream*)pvContext;
        ptr->m_callback->Close();
    }

    std::shared_ptr<PullAudioInputStreamCallback> m_callback;
};


inline std::shared_ptr<PushAudioInputStream> AudioInputStream::CreatePushStream()
{
    return PushAudioInputStream::Create();
}

inline std::shared_ptr<PushAudioInputStream> AudioInputStream::CreatePushStream(std::shared_ptr<AudioStreamFormat> format)
{
    return PushAudioInputStream::Create(format);
}

#if defined(BINDING_OBJECTIVE_C) || !defined(SWIG)
inline std::shared_ptr<PullAudioInputStream> AudioInputStream::CreatePullStream(void* pvContext, CUSTOM_AUDIO_PULL_STREAM_READ_CALLBACK readCallback, CUSTOM_AUDIO_PULL_STREAM_CLOSE_CALLBACK closeCallback)
{
    return PullAudioInputStream::Create(pvContext, readCallback, closeCallback);
}

inline std::shared_ptr<PullAudioInputStream> AudioInputStream::CreatePullStream(ReadCallbackFunction_Type readCallback, CloseCallbackFunction_Type closeCallback)
{
    return PullAudioInputStream::Create(readCallback, closeCallback);
}
#endif // defined(BINDING_OBJECTIVE_C) || !defined(SWIG)

inline std::shared_ptr<PullAudioInputStream> AudioInputStream::CreatePullStream(std::shared_ptr<PullAudioInputStreamCallback> callback)
{
    return PullAudioInputStream::Create(callback);
}

#if defined(BINDING_OBJECTIVE_C) || !defined(SWIG)
inline std::shared_ptr<PullAudioInputStream> AudioInputStream::CreatePullStream(std::shared_ptr<AudioStreamFormat> format, void* pvContext, CUSTOM_AUDIO_PULL_STREAM_READ_CALLBACK readCallback, CUSTOM_AUDIO_PULL_STREAM_CLOSE_CALLBACK closeCallback)
{
    return PullAudioInputStream::Create(format, pvContext, readCallback, closeCallback);
}

inline std::shared_ptr<PullAudioInputStream> AudioInputStream::CreatePullStream(std::shared_ptr<AudioStreamFormat> format, ReadCallbackFunction_Type readCallback, CloseCallbackFunction_Type closeCallback)
{
    return PullAudioInputStream::Create(format, readCallback, closeCallback);
}
#endif // defined(BINDING_OBJECTIVE_C) || !defined(SWIG)

inline std::shared_ptr<PullAudioInputStream> AudioInputStream::CreatePullStream(std::shared_ptr<AudioStreamFormat> format, std::shared_ptr<PullAudioInputStreamCallback> callback)
{
    return PullAudioInputStream::Create(format, callback);
}


/// <summary>
/// Represents audio output stream used for custom audio output configurations.
/// Added in version 1.4.0
/// </summary>
class AudioOutputStream
{
public:

#if defined(BINDING_OBJECTIVE_C) || !defined(SWIG)
    using WriteCallbackFunction_Type = ::std::function<int(uint8_t*, uint32_t)>;
    using CloseCallbackFunction_Type = ::std::function<void()>;
#endif // defined(BINDING_OBJECTIVE_C) || !defined(SWIG)

    /// <summary>
    /// Internal operator used to get underlying handle value.
    /// </summary>
    explicit operator SPXAUDIOSTREAMHANDLE() const { return m_haudioStream.get(); }

    /// <summary>
    /// Creates a memory backed PullAudioOutputStream using the default format (16Khz 16bit mono PCM).
    /// </summary>
    /// <returns>A shared pointer to PullAudioOutputStream</returns>
    static std::shared_ptr<PullAudioOutputStream> CreatePullStream();

    /// <summary>
    /// Creates a memory backed PullAudioOutputStream with the specified audio format.
    /// </summary>
    /// <param name="format">Audio stream format.</param>
    /// <returns>A shared pointer to PullAudioOutputStream</returns>
    static std::shared_ptr<PullAudioOutputStream> CreatePullStream(std::shared_ptr<AudioStreamFormat> format);

#if defined(BINDING_OBJECTIVE_C) || !defined(SWIG)
    /// <summary>
    /// Creates a PushAudioOutputStream that delegates to the specified callback functions for Write() and Close() methods, using the default format (16Khz 16bit mono PCM).
    /// </summary>
    /// <param name="pvContext">Context pointer to use when invoking the callbacks.</param>
    /// <param name="writeCallback">Write callback.</param>
    /// <param name="closeCallback">Close callback.</param>
    /// <returns>A shared pointer to PushAudioOutputStream</returns>
    static std::shared_ptr<PushAudioOutputStream> CreatePushStream(void* pvContext, CUSTOM_AUDIO_PUSH_STREAM_WRITE_CALLBACK writeCallback, CUSTOM_AUDIO_PUSH_STREAM_CLOSE_CALLBACK closeCallback = nullptr);

    /// <summary>
    /// Creates a PushAudioOutputStream that delegates to the specified callback functions for Write() and Close() methods, using the default format (16Khz 16bit mono PCM).
    /// </summary>
    /// <param name="writeCallback">Write callback.</param>
    /// <param name="closeCallback">Close callback.</param>
    /// <returns>A shared pointer to PushAudioOutputStream</returns>
    static std::shared_ptr<PushAudioOutputStream> CreatePushStream(WriteCallbackFunction_Type writeCallback, CloseCallbackFunction_Type closeCallback = nullptr);
#endif // defined(BINDING_OBJECTIVE_C) || !defined(SWIG)

    /// <summary>
    /// Creates a PushAudioOutputStream that delegates to the specified callback interface for Write() and Close() methods, using the default format (16Khz 16bit mono PCM).
    /// </summary>
    /// <param name="callback">Shared pointer to PushAudioOutputStreamCallback instance.</param>
    /// <returns>A shared pointer to PushAudioOutputStream</returns>
    static std::shared_ptr<PushAudioOutputStream> CreatePushStream(std::shared_ptr<PushAudioOutputStreamCallback> callback);

#if defined(BINDING_OBJECTIVE_C) || !defined(SWIG)
    /// <summary>
    /// Creates a PushAudioOutputStream that delegates to the specified callback functions for Write() and Close() methods.
    /// </summary>
    /// <param name="format">Audio stream format.</param>
    /// <param name="pvContext">Context pointer to use when invoking the callbacks.</param>
    /// <param name="writeCallback">Write callback.</param>
    /// <param name="closeCallback">Close callback.</param>
    /// <returns>A shared pointer to PushAudioOutputStream</returns>
    static std::shared_ptr<PushAudioOutputStream> CreatePushStream(std::shared_ptr<AudioStreamFormat> format, void* pvContext, CUSTOM_AUDIO_PUSH_STREAM_WRITE_CALLBACK writeCallback, CUSTOM_AUDIO_PUSH_STREAM_CLOSE_CALLBACK closeCallback = nullptr);

    /// <summary>
    /// Creates a PushAudioOutputStream that delegates to the specified callback functions for Write() and Close() methods.
    /// </summary>
    /// <param name="format">Audio stream format.</param>
    /// <param name="writeCallback">Write callback.</param>
    /// <param name="closeCallback">Close callback.</param>
    /// <returns>A shared pointer to PushAudioOutputStream</returns>
    static std::shared_ptr<PushAudioOutputStream> CreatePushStream(std::shared_ptr<AudioStreamFormat> format, WriteCallbackFunction_Type writeCallback, CloseCallbackFunction_Type closeCallback = nullptr);
#endif // defined(BINDING_OBJECTIVE_C) || !defined(SWIG)

    /// <summary>
    /// Creates a PushAudioOutputStream that delegates to the specified callback interface for Write() and Close() methods, using the specified format.
    /// </summary>
    /// <param name="format">Audio stream format.</param>
    /// <param name="callback">Shared pointer to PushAudioOutputStreamCallback instance.</param>
    /// <returns>A shared pointer to PushAudioOutputStream</returns>
    static std::shared_ptr<PushAudioOutputStream> CreatePushStream(std::shared_ptr<AudioStreamFormat> format, std::shared_ptr<PushAudioOutputStreamCallback> callback);

protected:

    /*! \cond PROTECTED */

    /// <summary>
    /// Internal constructor. Creates a new instance using the provided handle.
    /// </summary>
    explicit AudioOutputStream(SPXAUDIOSTREAMHANDLE haudioStream) : m_haudioStream(haudioStream) { }

    /// <summary>
    /// Destructor, does nothing.
    /// </summary>
    virtual ~AudioOutputStream() {}

    /// <summary>
    /// Internal helper method to get the default format if the specified format is nullptr.
    /// </summary>
    static std::shared_ptr<AudioStreamFormat> UseDefaultFormatIfNull(std::shared_ptr<AudioStreamFormat> format) { return format != nullptr ? format : AudioStreamFormat::GetDefaultInputFormat(); }

    /// <summary>
    /// Internal helper method to get the audio stream format handle.
    /// </summary>
    static SPXAUDIOSTREAMFORMATHANDLE GetFormatHandle(std::shared_ptr<AudioStreamFormat> format) { return (SPXAUDIOSTREAMFORMATHANDLE)(*format.get()); }

    /// <summary>
    /// Internal member variable that holds the smart handle.
    /// </summary>
    SmartHandle<SPXAUDIOSTREAMHANDLE, &audio_stream_release> m_haudioStream;

    /*! \endcond */

private:

    DISABLE_COPY_AND_MOVE(AudioOutputStream);
};


/// <summary>
/// Represents memory backed pull audio output stream used for custom audio output.
/// Added in version 1.4.0
/// </summary>
class PullAudioOutputStream : public AudioOutputStream
{
public:

    /// <summary>
    /// Creates a memory backed PullAudioOutputStream using the default format (16Khz 16bit mono PCM).
    /// </summary>
    /// <returns>A shared pointer to PullAudioOutputStream</returns>
    static std::shared_ptr<PullAudioOutputStream> Create()
    {
        return Create(nullptr);
    }

    /// <summary>
    /// Creates a memory backed PullAudioOutputStream with the specified audio format.
    /// </summary>
    /// <param name="format">Audio stream format.</param>
    /// <returns>A shared pointer to PullAudioOutputStream</returns>
    static std::shared_ptr<PullAudioOutputStream> Create(std::shared_ptr<AudioStreamFormat> format)
    {
        format = UseDefaultFormatIfNull(format);

        SPXAUDIOSTREAMHANDLE haudioStream = SPXHANDLE_INVALID;
        SPX_THROW_ON_FAIL(audio_stream_create_pull_audio_output_stream(&haudioStream, GetFormatHandle(format)));

        auto stream = new PullAudioOutputStream(haudioStream);
        return std::shared_ptr<PullAudioOutputStream>(stream);
    }

    /// <summary>
    /// Reads a chunk of the audio data and fill it to given buffer
    /// </summary>
    /// <param name="buffer">A buffer to receive read data.</param>
    /// <param name="bufferSize">Size of the buffer.</param>
    /// <returns>Size of data filled to the buffer, 0 means end of stream</returns>
    uint32_t Read(uint8_t* buffer, uint32_t bufferSize)
    {
        uint32_t filledSize = 0;
        SPX_THROW_ON_FAIL(pull_audio_output_stream_read(m_haudioStream, buffer, bufferSize, &filledSize));

        return filledSize;
    }


protected:

    /*! \cond PROTECTED */

    /// <summary>
    /// Internal constructor. Creates a new instance using the provided handle.
    /// </summary>
    explicit PullAudioOutputStream(SPXAUDIOSTREAMHANDLE haudioStream) : AudioOutputStream(haudioStream) { }

    /*! \endcond */


private:

    template <class T>
    static std::shared_ptr<T> SpxAllocSharedBuffer(size_t sizeInBytes)
    {
        auto ptr = reinterpret_cast<T*>(new uint8_t[sizeInBytes]);
        auto deleter = [](T* p) { delete[] reinterpret_cast<uint8_t*>(p); };

        std::shared_ptr<T> buffer(ptr, deleter);
        return buffer;
    }


private:

    DISABLE_COPY_AND_MOVE(PullAudioOutputStream);

    std::vector<uint8_t> m_audioData;
};


/// <summary>
/// An interface that defines callback methods for an audio output stream.
/// Added in version 1.4.0
/// </summary>
/// <remarks>
/// Derive from this class and implement its function to provide your own
/// data as an audio output stream.
/// <remarks>
class PushAudioOutputStreamCallback
{
public:

    /// <summary>
    /// Destructor, does nothing.
    /// </summary>
    virtual ~PushAudioOutputStreamCallback() {}

    /// <summary>
    /// This function is called to synchronously put data to the audio stream.
    /// </summary>
    /// <param name="dataBuffer">The pointer to the buffer from which to consume the audio data.</param>
    /// <param name="size">The size of the buffer.</param>
    /// <returns>The number of bytes consumed from the buffer</returns>
    virtual int Write(uint8_t* dataBuffer, uint32_t size) = 0;

    /// <summary>
    /// This function is called to close the audio stream.
    /// </summary>
    virtual void Close() = 0;

protected:

    /*! \cond PROTECTED */

    /// <summary>
    /// Constructor, does nothing.
    /// </summary>
    PushAudioOutputStreamCallback() {};

    /*! \endcond */

private:

    DISABLE_COPY_AND_MOVE(PushAudioOutputStreamCallback);
};


/// <summary>
/// Push audio output stream class.
/// Added in version 1.4.0
/// </summary>
class PushAudioOutputStream : public AudioOutputStream
{
public:

#if defined(BINDING_OBJECTIVE_C) || !defined(SWIG)
    /// <summary>
    /// Creates a PushAudioOutputStream utilizing the specified Write() and Close() "C" callback functions pointers
    /// </summary>
    /// <param name="pvContext">Context pointer to use when invoking the callbacks.</param>
    /// <param name="writeCallback">Write callback.</param>
    /// <param name="closeCallback">Close callback.</param>
    /// <returns>A shared pointer to PushAudioOutputStream</returns>
    static std::shared_ptr<PushAudioOutputStream> Create(void* pvContext, CUSTOM_AUDIO_PUSH_STREAM_WRITE_CALLBACK writeCallback, CUSTOM_AUDIO_PUSH_STREAM_CLOSE_CALLBACK closeCallback = nullptr)
    {
        return Create(nullptr, pvContext, writeCallback, closeCallback);
    }

    /// <summary>
    /// Creates a PushAudioOutputStream utilizing the specified Write() and Close() callback functions.
    /// </summary>
    /// <param name="writeCallback">Write callback.</param>
    /// <param name="closeCallback">Close callback.</param>
    /// <returns>A shared pointer to PushAudioOutputStream</returns>
    static std::shared_ptr<PushAudioOutputStream> Create(WriteCallbackFunction_Type writeCallback, CloseCallbackFunction_Type closeCallback = nullptr)
    {
        return Create(nullptr, writeCallback, closeCallback);
    }
#endif // defined(BINDING_OBJECTIVE_C) || !defined(SWIG)

    /// <summary>
    /// Creates a PushAudioOutputStream utilizing the specified callback interface with Write() and Close() callback function.
    /// </summary>
    /// <param name="callback">Shared pointer to PushAudioOutputStreamCallback instance.</param>
    /// <returns>A shared pointer to PushAudioOutputStream</returns>
    static std::shared_ptr<PushAudioOutputStream> Create(std::shared_ptr<PushAudioOutputStreamCallback> callback)
    {
        return Create(nullptr, callback);
    }

#if defined(BINDING_OBJECTIVE_C) || !defined(SWIG)
    /// <summary>
    /// Creates a PushAudioOutputStream utilizing the specified Write() and Close() "C" callback functions pointers
    /// </summary>
    /// <param name="format">Audio stream format.</param>
    /// <param name="pvContext">Context pointer to use when invoking the callbacks.</param>
    /// <param name="writeCallback">Write callback.</param>
    /// <param name="closeCallback">Close callback.</param>
    /// <returns>A shared pointer to PushAudioOutputStream</returns>
    static std::shared_ptr<PushAudioOutputStream> Create(std::shared_ptr<AudioStreamFormat> format, void* pvContext, CUSTOM_AUDIO_PUSH_STREAM_WRITE_CALLBACK writeCallback, CUSTOM_AUDIO_PUSH_STREAM_CLOSE_CALLBACK closeCallback = nullptr)
    {
        return Create(format,
            [=](uint8_t* buffer, uint32_t size) -> int { return writeCallback(pvContext, buffer, size); },
            [=]() { if (closeCallback != nullptr) { closeCallback(pvContext); } });
    }

    /// <summary>
    /// Creates a PushAudioOutputStream utilizing the specified Write() and Close() callback functions.
    /// </summary>
    /// <param name="format">Audio stream format.</param>
    /// <param name="writeCallback">Write callback.</param>
    /// <param name="closeCallback">Close callback.</param>
    /// <returns>A shared pointer to PushAudioOutputStream</returns>
    static std::shared_ptr<PushAudioOutputStream> Create(std::shared_ptr<AudioStreamFormat> format, WriteCallbackFunction_Type writeCallback, CloseCallbackFunction_Type closeCallback = nullptr)
    {
        auto wrapper = std::make_shared<FunctionCallbackWrapper>(writeCallback, closeCallback);
        return Create(format, wrapper);
    }
#endif // defined(BINDING_OBJECTIVE_C) || !defined(SWIG)

    /// <summary>
    /// Creates a PushAudioOutputStream utilizing the specified Write() and Close() callback functions.
    /// </summary>
    /// <param name="format">Audio stream format.</param>
    /// <param name="callback">Shared pointer to PushAudioOutputStreamCallback instance.</param>
    /// <returns>A shared pointer to PushAudioOutputStream</returns>
    static std::shared_ptr<PushAudioOutputStream> Create(std::shared_ptr<AudioStreamFormat> format, std::shared_ptr<PushAudioOutputStreamCallback> callback)
    {
        format = UseDefaultFormatIfNull(format);

        SPXAUDIOSTREAMHANDLE haudioStream = SPXHANDLE_INVALID;
        SPX_THROW_ON_FAIL(audio_stream_create_push_audio_output_stream(&haudioStream, GetFormatHandle(format)));

        auto stream = new PushAudioOutputStream(haudioStream);
        SPX_THROW_ON_FAIL(push_audio_output_stream_set_callbacks(haudioStream, stream, WriteCallbackWrapper, CloseCallbackWrapper));
        stream->m_callback = callback;

        return std::shared_ptr<PushAudioOutputStream>(stream);
    }

protected:

    /*! \cond PROTECTED */

    /// <summary>
    /// Internal constructor. Creates a new instance using the provided handle.
    /// </summary>
    explicit PushAudioOutputStream(SPXAUDIOSTREAMHANDLE haudioStream) : AudioOutputStream(haudioStream) { }

#if defined(BINDING_OBJECTIVE_C) || !defined(SWIG)
    class FunctionCallbackWrapper : public PushAudioOutputStreamCallback
    {
    public:

        FunctionCallbackWrapper(WriteCallbackFunction_Type writeCallback, CloseCallbackFunction_Type closeCallback) :
            m_writeCallback(writeCallback),
            m_closeCallback(closeCallback)
        {
        };

        int Write(uint8_t* dataBuffer, uint32_t size) override { return m_writeCallback(dataBuffer, size); }
        void Close() override { if (m_closeCallback != nullptr) m_closeCallback(); };

    private:

        DISABLE_COPY_AND_MOVE(FunctionCallbackWrapper);

        WriteCallbackFunction_Type m_writeCallback;
        CloseCallbackFunction_Type m_closeCallback;
    };
#endif // defined(BINDING_OBJECTIVE_C) || !defined(SWIG)

    /*! \endcond */

private:

    DISABLE_COPY_AND_MOVE(PushAudioOutputStream);

    static int WriteCallbackWrapper(void* pvContext, uint8_t* dataBuffer, uint32_t size)
    {
        PushAudioOutputStream* ptr = (PushAudioOutputStream*)pvContext;
        return ptr->m_callback->Write(dataBuffer, size);
    }

    static void CloseCallbackWrapper(void* pvContext)
    {
        SPX_DBG_TRACE_SCOPE(__FUNCTION__, __FUNCTION__);
        PushAudioOutputStream* ptr = (PushAudioOutputStream*)pvContext;
        ptr->m_callback->Close();
    }

    std::shared_ptr<PushAudioOutputStreamCallback> m_callback;
};


inline std::shared_ptr<PullAudioOutputStream> AudioOutputStream::CreatePullStream()
{
    return PullAudioOutputStream::Create();
}

inline std::shared_ptr<PullAudioOutputStream> AudioOutputStream::CreatePullStream(std::shared_ptr<AudioStreamFormat> format)
{
    return PullAudioOutputStream::Create(format);
}

#if defined(BINDING_OBJECTIVE_C) || !defined(SWIG)
inline std::shared_ptr<PushAudioOutputStream> AudioOutputStream::CreatePushStream(void* pvContext, CUSTOM_AUDIO_PUSH_STREAM_WRITE_CALLBACK writeCallback, CUSTOM_AUDIO_PUSH_STREAM_CLOSE_CALLBACK closeCallback)
{
    return PushAudioOutputStream::Create(pvContext, writeCallback, closeCallback);
}

inline std::shared_ptr<PushAudioOutputStream> AudioOutputStream::CreatePushStream(WriteCallbackFunction_Type writeCallback, CloseCallbackFunction_Type closeCallback)
{
    return PushAudioOutputStream::Create(writeCallback, closeCallback);
}
#endif // defined(BINDING_OBJECTIVE_C) || !defined(SWIG)

inline std::shared_ptr<PushAudioOutputStream> AudioOutputStream::CreatePushStream(std::shared_ptr<PushAudioOutputStreamCallback> callback)
{
    return PushAudioOutputStream::Create(callback);
}

#if defined(BINDING_OBJECTIVE_C) || !defined(SWIG)
inline std::shared_ptr<PushAudioOutputStream> AudioOutputStream::CreatePushStream(std::shared_ptr<AudioStreamFormat> format, void* pvContext, CUSTOM_AUDIO_PUSH_STREAM_WRITE_CALLBACK writeCallback, CUSTOM_AUDIO_PUSH_STREAM_CLOSE_CALLBACK closeCallback)
{
    return PushAudioOutputStream::Create(format, pvContext, writeCallback, closeCallback);
}

inline std::shared_ptr<PushAudioOutputStream> AudioOutputStream::CreatePushStream(std::shared_ptr<AudioStreamFormat> format, WriteCallbackFunction_Type writeCallback, CloseCallbackFunction_Type closeCallback)
{
    return PushAudioOutputStream::Create(format, writeCallback, closeCallback);
}
#endif // defined(BINDING_OBJECTIVE_C) || !defined(SWIG)

inline std::shared_ptr<PushAudioOutputStream> AudioOutputStream::CreatePushStream(std::shared_ptr<AudioStreamFormat> format, std::shared_ptr<PushAudioOutputStreamCallback> callback)
{
    return PushAudioOutputStream::Create(format, callback);
}


} } } } // Microsoft::CognitiveServices::Speech::Audio
