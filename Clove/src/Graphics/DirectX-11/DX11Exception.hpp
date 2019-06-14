#pragma once

#include "Clove/Exception/CloveException.hpp"

namespace clv::gfx{
	class DX11Exception : public CloveException{
		//VARIABLES
	public:
		HRESULT hr;
		std::string info;

		//FUNCTIONS
	public:
		DX11Exception() = delete;
		DX11Exception(const DX11Exception& other) = delete;
		DX11Exception(DX11Exception&& other) noexcept = delete;
		CloveException& operator=(const CloveException& other) = delete;
		CloveException& operator=(CloveException&& other) noexcept = delete;
		virtual ~DX11Exception();

		DX11Exception(int lineNum, const char* file, HRESULT hr, const std::vector<std::string>& messages = {});

		virtual const char* what() const noexcept override;
		virtual const char* getType() const noexcept override;

		HRESULT getErrorCode() const noexcept;

		std::string getErrorString() const noexcept;
		std::string getErrorDescription() const noexcept;
		std::string getErrorInfo() const noexcept;
	};

	class DeviceRemovedException : public DX11Exception{
		using DX11Exception::DX11Exception;

		//VARIABLES
	private:
		std::string reason;

		//FUNCTIONS
	public:
		DeviceRemovedException() = delete;
		DeviceRemovedException(const DeviceRemovedException& other) = delete;
		DeviceRemovedException(DeviceRemovedException&& other) noexcept = delete;
		DeviceRemovedException& operator=(const DeviceRemovedException& other) = delete;
		DeviceRemovedException& operator=(DeviceRemovedException&& other) noexcept = delete;
		~DeviceRemovedException();

		virtual const char* getType() const noexcept override;
	};

	class InfoException : public CloveException{
		//VARIABLES
	private:
		std::string info;

		//FUNCTIONS
	public:
		InfoException() = delete;
		InfoException(const InfoException& other) = delete;
		InfoException(InfoException&& other) noexcept = delete;
		InfoException& operator=(const InfoException& other) = delete;
		InfoException& operator=(InfoException&& other) noexcept = delete;
		~InfoException();

		InfoException(int lineNum, const char* file, const std::vector<std::string>& messages);

		virtual const char* what() const noexcept override;
		virtual const char* getType() const noexcept override;

		std::string getErrorInfo() const noexcept;
	};
}

#define DX11_EXCEPT_NOINFO(hr) clv::gfx::DX11Exception(__LINE__, __FILE__, (hr))
#define DX11_THROW_NOINFO(hrcall) { if(FAILED(hr = (hrcall))){ throw DX11_EXCEPT_NOINFO(hr); } }

#if CLV_DEBUG
	#define DX11_EXCEPT(hr) clv::gfx::DX11Exception(__LINE__, __FILE__, (hr), infoManager.getMessages())
	#define DX11_THROW_INFO(hrcall) infoManager.set(); if(FAILED(hr = (hrcall))) throw DX11_EXCEPT(hr)
	#define DX11_DEVICE_REMOVED_EXCPTION(hr) clv::gfx::DeviceRemovedException(__LINE__, __FILE__, (hr), infoManager.getMessages())
	#define DX11_THROW_INFO_ONLY(call) infoManager.set(); (call); { auto v = infoManager.getMessages(); if(!v.empty()){ throw clv::gfx::InfoException(__LINE__, __FILE__, v); } }

	#define DX11_INFO_PROVIDER(renderer) HRESULT hr; DXGIInfoManager& infoManager = renderer->getInfoManager();
#else
	#define DX11_EXCEPT(hr) clv::gfx::DX11Exception(__LINE__, __FILE__, (hr))
	#define DX11_THROW_INFO(hrcall) DX11_THROW_NOINFO(hrcall)
	#define DX11_DEVICE_REMOVED_EXCPTION(hr) clv::gfx::DeviceRemovedException(__LINE__, __FILE__, (hr))
	#define DX11_THROW_INFO_ONLY(call) (call)

	#define DX11_INFO_PROVIDER(renderer) HRESULT hr;
#endif