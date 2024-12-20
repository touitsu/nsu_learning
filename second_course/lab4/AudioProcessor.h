#pragma once

#include <iostream>
#include <fstream>
#include <filesystem>
#include <cstdint>
#include <string>
#include <immintrin.h>
#include <malloc.h>
#include <memory>
#include <vector>
#include <map>

#define BuffSize 32
#define PI 3.1415926535

namespace AudioProcessor {


	std::string intToBytes(size_t val);


	uint32_t bytesToInt(const std::string& bytes, int32_t bytesCount, int32_t startingPosition = 0);


	int32_t signedBytesToInt(const std::string& bytes, int32_t bytesCount, int32_t startingPosition = 0);


	int16_t* signedBytesToIntArray(const std::string& bytes, int32_t bytesCount);


	class Stream {
		
	protected:
		std::ifstream file;
		const std::string& path;
		std::string readBuff;

	public:

		Stream(const std::string& filePath);


		void open();


		virtual ~Stream();


		void read(int32_t size);


		bool isFileGood() const noexcept;


		const std::string& getPath() const noexcept;


		const std::string& getReadBuff() const noexcept;


		void setPos(int32_t pos);
	};

	class AudioStream : public virtual Stream {

	private:
		int32_t fileSize;
		int32_t duration;
		int32_t frequency;
		int32_t bytesPerSec;
		int32_t bytesTillData;
		int32_t bitsPerSample;

	public:
		explicit AudioStream(std::string filePath);


		~AudioStream() {

		}


		const int32_t& getBytesPerSec() const noexcept;


		const int32_t& getBytesTillData() const noexcept;


		const int32_t& getFileSize() const noexcept;


		const int32_t& getDuration() const noexcept;


		const int32_t& getBitsPerSample() const noexcept;


		const int32_t& getFrequency() const noexcept;

	};

	class Processor {
	protected:
		AudioStream& stream;
		std::string& outputPath;
		std::ofstream outputFile;
		int32_t start;
		int32_t end;


		void write(const std::string& string, int32_t size);


		void dumpOriginal(int32_t seconds);

	public:
		Processor(AudioStream& stream, std::string& path, int32_t startPoint, int32_t endPoint = -1);


		virtual ~Processor();


		virtual void apply();

	};


	class SaveProcessor : public virtual Processor {
	private:

	public:
		SaveProcessor(AudioStream& stream, std::string& path);

		void apply() override;
	};


	class MuteProcessor : public virtual Processor {
	private:
		
	public:
		MuteProcessor(AudioStream& stream, std::string& path, int32_t startPoint, int32_t endPoint = -1);


		void apply() override;
	};

	class MixProcessor : public virtual Processor {
	private:
		AudioStream& secondStream;

		const std::string calculateMix(const std::string& bytes1, const std::string& bytes2) const noexcept;

	public:
		MixProcessor(AudioStream& stream1, AudioStream& stream2, std::string& path, int32_t startPoint, int32_t endPoint = -1);


		void apply() override;
	};


	class FrequencyProcessor : public virtual Processor {
	private:
		
		struct complexNum {
			float real;
			float imaginary;


			complexNum(float r = 0, float i = 0);


			complexNum operator+(complexNum& b);


			complexNum operator-(complexNum& b);


			complexNum operator*(complexNum& b);


			void operator*=(float b);

		};


		std::vector<complexNum> fft(std::vector<complexNum>& signal, bool invert);


	public:
		FrequencyProcessor(AudioStream& stream, std::string& path, int32_t startFrequency, int32_t endFrequnecy = -1);


		void apply() override;
	};

	
	class EchoProcessor : public virtual Processor{
	private:

		const std::string calculateMix(const std::string& bytes1, const std::string& bytes2) const noexcept;

	public:
		EchoProcessor(AudioStream& stream1, std::string& path, int32_t startPoint, int32_t endPoint = -1);

		void apply();
	};


	class Factory {
	public:
		virtual std::unique_ptr<Processor> createProcessor(std::vector<std::shared_ptr<AudioStream>>& streams, std::string& outputPath, int32_t startPoint, int32_t endPoint);
	};


	class SaveFactory : public virtual Factory {
	public:
		std::unique_ptr<Processor> createProcessor(std::vector<std::shared_ptr<AudioStream>>& streams, std::string& outputPath, int32_t startPoint, int32_t endPoint) override;
	};


	class MuteFactory : public virtual Factory {
	public:
		std::unique_ptr<Processor> createProcessor(std::vector<std::shared_ptr<AudioStream>>& streams, std::string& outputPath, int32_t startPoint, int32_t endPoint) override;
	};


	class MixFactory : public virtual Factory {
	public:
		std::unique_ptr<Processor> createProcessor(std::vector<std::shared_ptr<AudioStream>>& streams, std::string& outputPath, int32_t startPoint, int32_t endPoint) override;
	};


	class FftFactory : public virtual Factory {
	public:
		std::unique_ptr<Processor> createProcessor(std::vector<std::shared_ptr<AudioStream>>& streams, std::string& outputPath, int32_t startPoint, int32_t endPoint) override;
	};


	class EchoFactory : public virtual Factory {
	public:
		std::unique_ptr<Processor> createProcessor(std::vector<std::shared_ptr<AudioStream>>& streams, std::string& outputPath, int32_t startPoint, int32_t endPoint) override;
	};


	class ConfigHandler {
	private:
		std::string& path;
		std::string& outputPath;
		std::ifstream file;
		std::vector<std::shared_ptr<std::string>>& inputs;

		struct commandStruct {
			std::vector<int32_t> vals;
			std::vector<int32_t> links;
			std::string commandName;
		};

		commandStruct getLineInput(std::string& line, int32_t lineIdx);

		
		std::vector<std::reference_wrapper<const std::string>> getMapKeys(std::map<std::string, Factory*>& mapIn);

		
		void save(std::string& path, std::map<std::string, Factory*>& mapIn);


	public:
		ConfigHandler(std::string& outputPath, std::string& configPath, std::vector<std::shared_ptr<std::string>>& inputsVec);


		~ConfigHandler();


		void handleConfig();
	};
}
