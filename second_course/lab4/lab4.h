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


	std::string intToBytes(size_t val) {
		std::string res;

		for (size_t i = 0; val != 0; i++) {
			res.push_back(static_cast<uint8_t>(val & 0xFF));
			val >>= 8;
		}

		return res;
	}


	uint32_t bytesToInt(const std::string& bytes, int32_t bytesCount, int32_t startingPosition = 0) {
		uint32_t res;

		if (bytesCount > bytes.size()) {
			throw std::invalid_argument("Buffer size exceeded.\n");
		}

		res = 0;

		for (size_t i = startingPosition + bytesCount - 1; i > startingPosition; i--) {
			res += static_cast<uint8_t>(bytes[i]);
			res <<= 8;
		}
		res += static_cast<uint8_t>(bytes[startingPosition]);

		return res;
	}


	int32_t signedBytesToInt(const std::string& bytes, int32_t bytesCount, int32_t startingPosition = 0) {
		int32_t res;

		if (bytesCount > bytes.size()) {
			throw std::invalid_argument("Buffer size exceeded.\n");
		}

		res = 0;

		for (size_t i = startingPosition + bytesCount - 1; i > startingPosition; i--) {
			res += static_cast<int8_t>(bytes[i]);
			res <<= 8;
		}
		res += static_cast<int8_t>(bytes[startingPosition]);

		return res;
	}


	int16_t* signedBytesToIntArray(const std::string& bytes, int32_t bytesCount) {
		int16_t* res;
		int32_t tmp;

		res = static_cast<int16_t*>(_aligned_malloc(sizeof(int16_t) * bytes.length() / bytesCount, sizeof(int16_t) * 8));

		for (size_t i = 0; i < bytes.length() / bytesCount; i++) {
			tmp = 0;

			for (size_t j = bytesCount * (i + 1) - 1; j > i * bytesCount; j--) {
				tmp += static_cast<int8_t>(bytes[j]);
				tmp <<= 8;
			}

			tmp += static_cast<int8_t>(bytes[i]);

			*(res + i) = tmp;
		}

		return res;
	}


	class Stream {
		
	protected:
		std::ifstream file;
		const std::string& path;
		std::string readBuff;

	public:

		Stream(const std::string& filePath) : path(filePath) {
			std::ofstream tmp;
			this->file.open(filePath, std::ios::binary);

			if (!this->file.is_open()) {
				throw std::invalid_argument((std::string("Can't open ") + this->path + " file.\n").c_str());
			}
		}


		void open() {
			this->file.open(this->path);

			if (!this->file.is_open()) {
				throw std::invalid_argument((std::string("Can't open ") + this->path + " file.\n").c_str());
			}
		}


		virtual ~Stream() {
			if (this->file.is_open()) {
				this->file.close();
			}
		}


		void read(int32_t size) {
			char charBuff[sizeof(size_t) * 8];

			if (size > sizeof(size_t) * 8) {
				throw std::invalid_argument((std::string("Can't read ") + std::to_string(size) + std::string(" bytes, maximum for this system is: ") + std::to_string(sizeof(size_t) * 8) + std::string("\n")).c_str());
			}

			this->file.read(charBuff, size);

			this->readBuff.assign(charBuff, size);
		}


		bool isFileGood() const noexcept {
			return this->file.good();
		}


		const std::string& getPath() const noexcept {
			return this->path;
		}


		const std::string& getReadBuff() const noexcept {
			return this->readBuff;
		}


		void setPos(int32_t pos) {
			file.seekg(pos, std::ios_base::beg);
		}
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
		explicit AudioStream(std::string filePath) : Stream(filePath) {
			std::ifstream headerFormatFile("./format.txt");
			static std::string templ = std::string("\u0052\u0049\u0046\u0046\u00E0\u0018\u004E\u0001\u0057\u0041\u0056\u0045\u0066\u006D\u0074\u0020\u0010\u0000\u0000\u0000\u0001\u0000\u0001\u0000\u0044\u00AC\u0000\u0000\u0088\u0058\u0001\u0000\u0002\u0000\u0010\u0000", 36); // header template, needed for file validation.
			int32_t bytesToSkip;

			bytesToSkip = 0;

			read(36);

			if (this->readBuff.substr(0, 4) != templ.substr(0, 4)) {
				throw std::invalid_argument("File header is invalid, must be RIFF.");
			}
			else if (this->readBuff.substr(8, 4) != templ.substr(8, 4)) {
				throw std::invalid_argument("File header is invalid, must be WAVE.");
			}
			else if (this->readBuff.substr(12, 4) != templ.substr(12, 4)) {
				throw std::invalid_argument("File header is invalid, must be fmt.");
			}
			else if (this->readBuff.substr(16, 1) != templ.substr(16, 1)) {
				throw std::invalid_argument("File header is invalid, block size is invalid.");
			}
			else if (this->readBuff.substr(20, 1) != templ.substr(20, 1)) {
				throw std::invalid_argument("Wrong audio format (Must be PCM).");
			}
			else if (this->readBuff.substr(22, 1) != templ.substr(22, 1)) {
				throw std::invalid_argument("Wrong number of channels (Must be mono).");
			}
			else if (this->readBuff.substr(24, 4) != templ.substr(24, 4)) {
				throw std::invalid_argument("Wrong frequency (Must be 44100 hz).");
			}

			this->frequency = bytesToInt(getReadBuff(), 4, 24);

			this->bytesPerSec = bytesToInt(getReadBuff(), 4, 28);

			this->bitsPerSample = bytesToInt(getReadBuff(), 2, 34);

			read(8);

			if (this->readBuff.substr(0, 4) == std::string("LIST")) {
				bytesToSkip = bytesToInt(getReadBuff(), 4, 4) + 8;

				std::cout << "Skipping " << bytesToSkip << " bytes of metadata." << std::endl;

				setPos(36 + bytesToSkip);

				read(8);
			}

			std::cout << this->readBuff << std::endl;

			this->fileSize = bytesToInt(getReadBuff(), 4, 4);

			this->duration = this->fileSize / this->bytesPerSec;

			this->bytesTillData = bytesToSkip + 36;
			
			this->readBuff.reserve(BuffSize);
		}


		~AudioStream() {

		}


		const int32_t& getBytesPerSec() const noexcept {
			return this->bytesPerSec;
		}


		const int32_t& getBytesTillData() const noexcept {
			return this->bytesTillData;
		}


		const int32_t& getFileSize() const noexcept {
			return fileSize;
		}


		const int32_t& getDuration() const noexcept {
			return this->duration;
		}


		const int32_t& getBitsPerSample() const noexcept {
			return this->bitsPerSample;
		}


		const int32_t& getFrequency() const noexcept {
			return this->frequency;
		}

	};

	class Processor {
	protected:
		AudioStream& stream;
		std::string& outputPath;
		std::ofstream outputFile;
		int32_t start;
		int32_t end;


		void write(const std::string& string, int32_t size) {
			this->outputFile.write(string.c_str(), size);
		}


		void dumpOriginal(int32_t seconds) {
			for (size_t i = 0; i < seconds; i++) {
				for (size_t j = 0; j < this->stream.getBytesPerSec() / BuffSize; j++) {
					this->stream.read(BuffSize);
					write(this->stream.getReadBuff(), BuffSize);
				}
			}
		}

	public:
		Processor(AudioStream& stream, std::string& path, int32_t startPoint, int32_t endPoint = -1) : stream(stream), outputPath(path) {
			this->outputFile.open(this->outputPath, std::ios_base::binary);

			if (!this->outputFile.is_open()) {
				throw std::invalid_argument((std::string("Can't create ") + this->outputPath + std::string(" file.\n")).c_str());
			}


			this->outputFile << "RIFF";

			this->outputFile << intToBytes(this->stream.getFileSize() + 34);
			for (size_t i = intToBytes(this->stream.getFileSize()).length(); i < 4; i++) {
				this->outputFile << '\u0000';
			}

			this->outputFile.write("WAVEfmt \u0010\u0000\u0000\u0000\u0001\u0000\u0001\u0000", 16);

			this->outputFile << intToBytes(this->stream.getFrequency()).c_str();
			for (size_t i = intToBytes(this->stream.getFrequency()).length(); i < 4; i++) {
				this->outputFile << '\u0000';
			}

			this->outputFile << intToBytes(this->stream.getBytesPerSec()).c_str();
			for (size_t i = intToBytes(this->stream.getBytesPerSec()).length(); i < 4; i++) {
				this->outputFile << '\u0000';
			}

			this->outputFile << intToBytes(this->stream.getBytesPerSec() / this->stream.getFrequency()).c_str();
			for (size_t i = intToBytes(this->stream.getBytesPerSec() / this->stream.getFrequency()).length(); i < 2; i++) {
				this->outputFile << '\u0000';
			}

			this->outputFile.write("\u0010\u0000data", 6);

			this->outputFile.write(intToBytes(this->stream.getFileSize()).c_str(), intToBytes(this->stream.getFileSize()).length());
			for (size_t i = intToBytes(this->stream.getFileSize()).length(); i < 4; i++) {
				this->outputFile << '\u0000';
			}

			this->start = startPoint;
			this->end = endPoint;
		}


		virtual ~Processor() {
			if (this->outputFile.is_open()) {
				this->outputFile.close();
			}
		}


		virtual void apply() {
			throw std::runtime_error("Base processor class can't do anything with the file.");
		}

	};


	class SaveProcessor : public virtual Processor {
	private:

	public:
		SaveProcessor(AudioStream& stream, std::string& path) : Processor(stream, path, 0, 0) {

		}

		void apply() override {
			dumpOriginal(stream.getDuration());
		}
	};


	class MuteProcessor : public virtual Processor {
	private:
		
	public:
		MuteProcessor(AudioStream& stream, std::string& path, int32_t startPoint, int32_t endPoint = -1) : Processor(stream, path, startPoint, endPoint) {

		}


		void apply() override {
			std::string buff;

			buff.reserve(BuffSize);
			for (size_t i = 0; i < BuffSize; i++) {
				buff.push_back('\u0000');
			}

			this->stream.setPos(this->stream.getBytesTillData());

			if (this->end < this->start && this->end != -1) {
				throw std::invalid_argument("Invalid end point, end point must be bigger than start point.\n");
			}

			if (this->end > this->stream.getDuration()) {
				throw std::invalid_argument("Invalid end point, input file duration is smaller.\n");
			}

			dumpOriginal(this->start);

			if (this->end == -1) {

				for (size_t i = 0; i < this->stream.getDuration() - this->start; i++) {
					for (size_t j = 0; j < this->stream.getBytesPerSec() / BuffSize; j++) {
						write(buff, BuffSize);
					}
				}
			}

			else {

				for (size_t i = 0; i < (this->end - this->start); i++) {
					for (size_t j = 0; j < this->stream.getBytesPerSec() / BuffSize; j++) {
						write(buff, BuffSize);
					}
				}

				this->stream.setPos(this->end * this->stream.getBytesPerSec());

				dumpOriginal(this->stream.getDuration() - this->end);
			}
		}
	};

	class MixProcessor : public virtual Processor {
	private:
		AudioStream& secondStream;

		const std::string calculateMix(const std::string& bytes1, const std::string& bytes2) const noexcept {
			std::string res;
			int32_t tmp;
			int32_t bytes;

			bytes = this->stream.getBitsPerSample() / 8;

			if (BuffSize % 16 == 0) {
				int16_t* firstStreamArray;
				int16_t* secondStreamArray;
				int16_t* resArray;
				
				resArray = static_cast<int16_t*>(_aligned_malloc(sizeof(int16_t) * BuffSize / bytes, sizeof(int16_t) * 8));

				firstStreamArray = signedBytesToIntArray(bytes1, bytes);
				secondStreamArray = signedBytesToIntArray(bytes2, bytes);

				for (size_t i = 0; i < bytes1.length() / bytes; i += 16) {
					_mm256_storeu_epi16(resArray + i * 16, (_mm256_add_epi16(_mm256_div_epi16(_mm256_loadu_epi16(firstStreamArray + i * 16), _mm256_set1_epi16(2)), 
																			 _mm256_div_epi16(_mm256_loadu_epi16(secondStreamArray + i * 16), _mm256_set1_epi16(2)))));
				}

				for (size_t i = 0; i < bytes1.length() / bytes; i++) {
					res.push_back(static_cast<int8_t>(*(resArray + i) & 0xFF));
					res.push_back(static_cast<int8_t>(*(resArray + i) >> 8));
				}

				_aligned_free(firstStreamArray);
				_aligned_free(secondStreamArray);
				_aligned_free(resArray);
			}

			else {
				for (size_t i = 0; i < bytes1.length() / (this->stream.getBitsPerSample() / 8); i++) {
					tmp = (signedBytesToInt(bytes1, bytes, i * bytes) + signedBytesToInt(bytes2, bytes, i * bytes)) / 2;
					res.push_back(static_cast<int8_t>(tmp & 0xFF));
					for (size_t j = 0; j < (this->stream.getBitsPerSample() / 8) - 1; j++) {
						res.push_back(static_cast<int8_t>((tmp >> 8) & 0xFF));
					}
				}
			}

			return std::as_const(res);
		}

	public:
		MixProcessor(AudioStream& stream1, AudioStream& stream2, std::string& path, int32_t startPoint, int32_t endPoint = -1) : Processor(stream1, path, startPoint, endPoint), secondStream(stream2) {

		}


		void apply() override {
			this->stream.setPos(this->stream.getBytesTillData() + 8);
			this->secondStream.setPos(this->secondStream.getBytesTillData() + 8);

			if (this->end < this->start && this->end != -1) {
				throw std::invalid_argument("Invalid end point, end point must be bigger than start point.\n");
			}

			if (this->end > this->stream.getDuration()) {
				throw std::invalid_argument("Invalid end point, input file duration is smaller.\n");
			}

			dumpOriginal(this->start);

			if (this->end == -1) {
				for (size_t i = 0; i < this->stream.getDuration() - this->start; i++) {
					for (size_t j = 0; j < this->stream.getBytesPerSec() / BuffSize; j++) {
						this->stream.read(BuffSize);
						if (this->secondStream.isFileGood()) {
							this->secondStream.read(BuffSize);

							write(calculateMix(this->stream.getReadBuff(), this->secondStream.getReadBuff()), BuffSize);
						}
						else {
							write(this->stream.getReadBuff(), BuffSize);
						}
					}
				}
			}
			else {
				for (size_t i = 0; i < this->end - this->start; i++) {
					for (size_t j = 0; j < this->stream.getBytesPerSec() / BuffSize; j++) {
						this->stream.read(BuffSize);
						this->secondStream.read(BuffSize);

						write(calculateMix(this->stream.getReadBuff(), this->secondStream.getReadBuff()), BuffSize);
					}
				}

				dumpOriginal(this->stream.getDuration() - this->end);
			}
		}
	};


	class FrequencyProcessor : public virtual Processor {
	private:
		
		struct complexNum {
			float real;
			float imaginary;


			complexNum(float r = 0, float i = 0) {
				this->real = r;
				this->imaginary = i;
			}


			complexNum operator+(complexNum& b) {
				complexNum res;

				res.real = this->real + b.real;
				res.imaginary = this->imaginary + b.imaginary;

				return res;
			}


			complexNum operator-(complexNum& b) {
				complexNum res;

				res.real = this->real - b.real;
				res.imaginary = this->imaginary - b.imaginary;

				return res;
			}


			complexNum operator*(complexNum& b) {
				complexNum res;

				res.real = this->real * b.real - this->imaginary * b.imaginary;
				res.imaginary = this->real * b.imaginary + b.real * this->imaginary;

				return res;
			}


			void operator*=(float b) {
				this->real = this->real * b;
				this->imaginary = this->imaginary * b;
			}

		};


		std::vector<complexNum> fft(std::vector<complexNum>& signal, bool invert) {
			std::vector<complexNum> even, odd, evenRes, oddRes, res;
			complexNum omega, omegan;

			if (signal.size() == 1) {
				return signal;
			}

			for (size_t i = 0; i < signal.size()/2; i++) {
				even.push_back(signal[i * 2]);
				odd.push_back(signal[i * 2 + 1]);
			}

			omega = complexNum(1.0f, 1.0f);
			omegan = complexNum(cos(2 * PI / signal.size() * (invert ? -1 : 1)), sin(2 * PI / signal.size() * (invert ? -1 : 1)));

			

			evenRes = fft(even, invert);
			oddRes = fft(odd, invert);
			
			res.reserve(signal.size());
			for (size_t i = 0; i < signal.size(); i++) {
				res.push_back(complexNum());
			}
			

			for (size_t i = 0, k1 = 0, k2 = 0; i < signal.size() / 2; i++, k1++, k2++) {
				res[i] = evenRes[k1] + omega * oddRes[k2];
				res[i + signal.size() / 2] = evenRes[k1] - omega * oddRes[k2];
				if (invert) {
					res[i] *= 0.5f;
					res[i + signal.size() / 2] *= 0.5f;
				}
				omega = omega * omegan;
			}

			return res;
		}


	public:
		FrequencyProcessor(AudioStream& stream, std::string& path, int32_t startFrequency, int32_t endFrequnecy = -1) : Processor(stream, path, startFrequency, endFrequnecy) {
		
		}

		void apply() override {
			std::vector<complexNum> signal;
			std::vector<complexNum> fftRes;
			int16_t* bytes;

			this->stream.setPos(this->stream.getBytesTillData() + 8);

			for (size_t sec = 0; sec < 2; sec++) {
				for (size_t i = 0; i < this->stream.getBytesPerSec() / BuffSize; i++) {
					this->stream.read(BuffSize);

					bytes = signedBytesToIntArray(stream.getReadBuff(), this->stream.getBitsPerSample() / 8);

					for (size_t j = 0; j < BuffSize / (this->stream.getBitsPerSample() / 8); j++) {
						signal.push_back(complexNum(static_cast<float>(*(bytes + j) & 0xFF), static_cast<float>(sec + (i * BuffSize + j) / this->stream.getBytesPerSec())));
						signal.push_back(complexNum(static_cast<float>(*(bytes + j) >> 8), static_cast<float>(sec + (i * BuffSize + j + 1) / this->stream.getBytesPerSec())));
					}
				}
				
				_aligned_free(bytes);
			}

			std::cout << signal.size() << std::endl;

			fftRes = fft(signal, 0);

			signal = fft(fftRes, 1);

			for (auto iter = signal.begin(); iter != signal.end(); iter++) {
				std::cout << "Real: " << iter->real << " Imaginary: " << iter->imaginary << std::endl;
			}
		}
	};

	
	class EchoProcessor : public virtual Processor{
	private:

		const std::string calculateMix(const std::string& bytes1, const std::string& bytes2) const noexcept {
			std::string res;
			int32_t tmp;
			int32_t bytes;

			bytes = this->stream.getBitsPerSample() / 8;

			if (BuffSize % 16 == 0) {
				int16_t* firstStreamArray;
				int16_t* secondStreamArray;
				int16_t* resArray;

				resArray = static_cast<int16_t*>(_aligned_malloc(sizeof(int16_t) * BuffSize / bytes, sizeof(int16_t) * 8));

				firstStreamArray = signedBytesToIntArray(bytes1, bytes);
				secondStreamArray = signedBytesToIntArray(bytes2, bytes);

				for (size_t i = 0; i < bytes1.length() / bytes; i += 16) {
					_mm256_storeu_epi16(resArray + i * 16, (_mm256_add_epi16(_mm256_div_epi16(_mm256_loadu_epi16(firstStreamArray + i * 16), _mm256_set1_epi16(2)),
						_mm256_div_epi16(_mm256_loadu_epi16(secondStreamArray + i * 16), _mm256_set1_epi16(2)))));
				}

				for (size_t i = 0; i < bytes1.length() / bytes; i++) {
					res.push_back(static_cast<int8_t>(*(resArray + i) & 0xFF));
					res.push_back(static_cast<int8_t>(*(resArray + i) >> 8));
				}

				_aligned_free(firstStreamArray);
				_aligned_free(secondStreamArray);
				_aligned_free(resArray);
			}

			else {
				for (size_t i = 0; i < bytes1.length() / (this->stream.getBitsPerSample() / 8); i++) {
					tmp = (signedBytesToInt(bytes1, bytes, i * bytes) + signedBytesToInt(bytes2, bytes, i * bytes)) / 2;
					res.push_back(static_cast<int8_t>(tmp & 0xFF));
					for (size_t j = 0; j < (this->stream.getBitsPerSample() / 8) - 1; j++) {
						res.push_back(static_cast<int8_t>((tmp >> 8) & 0xFF));
					}
				}
			}

			return std::as_const(res);
		}

	public:
		EchoProcessor(AudioStream& stream1, std::string& path, int32_t startPoint, int32_t endPoint = -1) : Processor(stream1, path, startPoint, endPoint) {

		}

		void apply() {
			std::string tmp;

			this->stream.setPos(this->stream.getBytesTillData() + 8);

			if (this->end < this->start && this->end != -1) {
				throw std::invalid_argument("Invalid end point, end point must be bigger than start point.\n");
			}

			if (this->end > this->stream.getDuration()) {
				throw std::invalid_argument("Invalid end point, input file duration is smaller.\n");
			}

			dumpOriginal(this->start);


			if (this->end == -1) {
				for (size_t i = 0; i < this->stream.getDuration() - this->start; i++) {
					for (size_t j = 0; j < this->stream.getBytesPerSec() / BuffSize; j++) {
						this->stream.setPos(this->stream.getBytesTillData() + 8 + (i + this->start) * this->stream.getBytesPerSec() + j * BuffSize);
						this->stream.read(BuffSize);
						tmp = std::string(this->stream.getReadBuff());
						this->stream.setPos(this->stream.getBytesTillData() + 8 + i * this->stream.getBytesPerSec() + j * BuffSize);
						this->stream.read(BuffSize);

						write(calculateMix(tmp, this->stream.getReadBuff()), BuffSize);
					}
				}
			}
			else {
				for (size_t i = 0; i < this->end - this->start; i++) {
					for (size_t j = 0; j < this->stream.getBytesPerSec() / BuffSize; j++) {
						this->stream.setPos(this->stream.getBytesTillData() + 8 + (i + this->start) * this->stream.getBytesPerSec() + j * BuffSize);
						this->stream.read(BuffSize);
						tmp = std::string(this->stream.getReadBuff());
						this->stream.setPos(this->stream.getBytesTillData() + 8 + i * this->stream.getBytesPerSec() + j * BuffSize);
						this->stream.read(BuffSize);

						write(calculateMix(tmp, this->stream.getReadBuff()), BuffSize);
					}
				}

				dumpOriginal(this->stream.getDuration() - this->end);
			}
		}
	};


	class Factory {
	public:
		virtual std::unique_ptr<Processor> createProcessor(std::vector<std::shared_ptr<AudioStream>>& streams, std::string& outputPath, int32_t startPoint, int32_t endPoint) {
			std::unique_ptr<Processor> res(new Processor(*streams[0].get(), outputPath, startPoint, endPoint));

			return res;
		}
	};


	class SaveFactory : public virtual Factory {
	public:
		std::unique_ptr<Processor> createProcessor(std::vector<std::shared_ptr<AudioStream>>& streams, std::string& outputPath, int32_t startPoint, int32_t endPoint) override {
			std::unique_ptr<Processor> res(new SaveProcessor(*streams[0].get(), outputPath));

			return res;
		}
	};


	class MuteFactory : public virtual Factory {
	public:
		std::unique_ptr<Processor> createProcessor(std::vector<std::shared_ptr<AudioStream>>& streams, std::string& outputPath, int32_t startPoint, int32_t endPoint) override {
			std::unique_ptr<Processor> res(new MuteProcessor(*streams[0].get(), outputPath, startPoint, endPoint));

			return res;
		}
	};


	class MixFactory : public virtual Factory {
	public:
		std::unique_ptr<Processor> createProcessor(std::vector<std::shared_ptr<AudioStream>>& streams, std::string& outputPath, int32_t startPoint, int32_t endPoint) override {
			std::unique_ptr<Processor> res(new MixProcessor(*streams[0].get(), *streams[1].get(), outputPath, startPoint, endPoint));

			return res;
		}
	};


	class FftFactory : public virtual Factory {
	public:
		std::unique_ptr<Processor> createProcessor(std::vector<std::shared_ptr<AudioStream>>& streams, std::string& outputPath, int32_t startPoint, int32_t endPoint) override {
			std::unique_ptr<Processor> res(new FrequencyProcessor(*streams[0].get(), outputPath, startPoint, endPoint));

			return res;
		}
	};


	class EchoFactory : public virtual Factory {
	public:
		std::unique_ptr<Processor> createProcessor(std::vector<std::shared_ptr<AudioStream>>& streams, std::string& outputPath, int32_t startPoint, int32_t endPoint) override {
			std::unique_ptr<Processor> res(new EchoProcessor(*streams[0].get(), outputPath, startPoint, endPoint));

			return res;
		}
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

		commandStruct getLineInput(std::string& line, int32_t lineIdx) {
			commandStruct res;
			std::vector<int32_t> vals, links;

			for (size_t i = line.find(' '); i < line.length(); i = line.find(' ', i + 1)) {
				if (line[i] == ' ') {
					if (line[i + 1] != '$') {
						if (line.find(' ', i) != std::string::npos) {
							vals.push_back(std::stoi(line.substr(i, line.find(' ', i))));
						}
						else {
							vals.push_back(std::stoi(line.substr(i, line.length())));
						}
					}
					else {
						if (line.find(' ', i) != std::string::npos) {
							links.push_back(std::stoi(line.substr(i + 2, line.find(' ', i))));
						}
						else {
							links.push_back(std::stoi(line.substr(i + 2, line.length())));
						}
					}
				}
			}

			if (vals.size() < 1) {
				throw std::invalid_argument((std::string("Not enough arguments provided in line: ") + std::to_string(lineIdx) + std::string("\n")).c_str());
			}
			if (links.size() == 0 && res.commandName == std::string("mix")) {
				throw std::invalid_argument((std::string("File to mix with isn't provided in line: ") + std::to_string(lineIdx) + std::string("\n").c_str()));
			}
			for (size_t i = 0; i < links.size(); i++) {
				if (links[i] > inputs.size()) {
					throw std::invalid_argument((std::string("Link to file is out of bounds in line: ") + std::to_string(lineIdx) + std::string("\n").c_str()));
				}
			}

			if (vals.size() == 1) {
				vals.push_back(-1);
			}

			res.vals = vals;
			res.links = links;
			res.commandName = line.substr(0, line.find(' '));

			return res;
		}

		
		std::vector<std::reference_wrapper<const std::string>> getMapKeys(std::map<std::string, Factory*>& mapIn) {
			std::vector<std::reference_wrapper<const std::string>> res;

			for (auto iter = mapIn.begin(); iter != mapIn.end(); iter++) {
				res.push_back(mapIn.find(iter->first)->first);
			}

			return res;
		}

		
		void save(std::string& path, std::map<std::string, Factory*>& mapIn) {
			std::vector<std::shared_ptr<AudioStream>> streams;

			std::shared_ptr<AudioStream> finalStream = std::make_shared<AudioStream>(path);

			streams.push_back(finalStream);

			mapIn["save"]->createProcessor(streams, this->outputPath, 0, 0)->apply();
		}


	public:
		ConfigHandler(std::string& outputPath, std::string& configPath, std::vector<std::shared_ptr<std::string>>& inputsVec) : path(configPath), outputPath(outputPath), inputs(inputsVec) {
			file.open(this->path);

			if (!file.is_open()) {
				throw std::invalid_argument((std::string("Can't open ") + this->path + std::string(" file.\n")).c_str());
			}
		}


		~ConfigHandler() {
			if (this->file.is_open()) {
				file.close();
			}
		}


		void handleConfig() {
			std::string line;
			commandStruct command;
			std::string curPath;
			std::string nextPath;
			std::map<std::string, Factory*> factories;
			std::vector<std::reference_wrapper<const std::string>> keys;
			int32_t lineIndex, procIndex, procIndexWas;

			lineIndex = 0;
			procIndex = 0;
			curPath = *inputs[0];
			nextPath = outputPath.substr(0, outputPath.find(".wav")) + std::to_string(procIndex) + std::string(".tmp");

			factories.insert(std::make_pair<std::string, Factory*>(std::string("mute"), new MuteFactory));
			factories.insert(std::make_pair<std::string, Factory*>(std::string("mix"), new MixFactory));
			//factories.insert(std::make_pair<std::string, Factory*>(std::string("fft"), new FftFactory));
			factories.insert(std::make_pair<std::string, Factory*>(std::string("save"), new SaveFactory));
			factories.insert(std::make_pair<std::string, Factory*>(std::string("echo"), new EchoFactory));
			
			keys = getMapKeys(factories);

			while (file.good()) {
				std::getline(file, line);

				if (line[0] != '#' && line[0] != '\n') {

					command = getLineInput(line, lineIndex);

					procIndexWas = procIndex;

					for (auto iter = keys.begin(); iter != keys.end(); iter++) {

						if ((*iter).get() == command.commandName) {
							std::vector<std::shared_ptr<AudioStream>> streams;

							std::shared_ptr<AudioStream> stream = std::make_shared<AudioStream>(curPath);

							streams.push_back(stream);

							for (size_t i = 1; i < inputs.size(); i++) {
								streams.push_back(std::make_shared<AudioStream>(*inputs[i]));
							}

							factories[command.commandName]->createProcessor(streams, nextPath, command.vals[0], command.vals[1]).get()->apply();

							procIndex++;

							curPath = nextPath;
							nextPath = outputPath.substr(0, outputPath.find(".wav")) + std::to_string(procIndex) + std::string(".tmp");
						}
					}

					if (procIndexWas == procIndex) {
						throw std::invalid_argument((std::string("Config is invalid, error occured in line: ") + std::to_string(lineIndex) + std::string(" Consider using a -h flag when launching the programm.\n")).c_str());
					}
				}

				lineIndex++;

				if (procIndex > 1) {
					std::remove((outputPath.substr(0, outputPath.find(".wav")) + std::to_string(procIndex - 2) + std::string(".tmp")).c_str());
				}
			}

			save(curPath, factories);

			std::remove(curPath.c_str());
		}
	};
}
