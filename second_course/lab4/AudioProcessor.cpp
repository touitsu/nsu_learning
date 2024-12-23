#include "AudioProcessor.h"
#include "exception.h"

using namespace AudioProcessor;


std::string AudioProcessor::intToBytes(size_t val) {
	std::string res;

	for (size_t i = 0; val != 0; i++) {
		res.push_back(static_cast<uint8_t>(val & 0xFF));
		val >>= 8;
	}

	return res;
}


uint32_t AudioProcessor::bytesToInt(const std::string& bytes, int32_t bytesCount, int32_t startingPosition) {
	uint32_t res;

	if (bytesCount > bytes.size()) {
		throw Exception(std::string("AudioProcessor::bytesToInt"), std::string("Invalid argument"), std::string("Buffer size exceeded."));
	}

	res = 0;

	for (size_t i = startingPosition + bytesCount - 1; i > startingPosition; i--) {
		res += static_cast<uint8_t>(bytes[i]);
		res <<= 8;
	}
	res += static_cast<uint8_t>(bytes[startingPosition]);

	return res;
}


int32_t AudioProcessor::signedBytesToInt(const std::string& bytes, int32_t bytesCount, int32_t startingPosition) {
	int32_t res;

	if (bytesCount > bytes.size()) {
		throw Exception(std::string("AudioProcessor::signedBytesToInt"), std::string("Invalid argument"), std::string("Buffer size exceeded."));
	}

	res = 0;

	for (size_t i = startingPosition + bytesCount - 1; i > startingPosition; i--) {
		res += static_cast<int8_t>(bytes[i]);
		res <<= 8;
	}
	res += static_cast<int8_t>(bytes[startingPosition]);

	std::cout << static_cast<signed char>(bytes[startingPosition]) << '\n';
	
	return res;
}


int16_t* AudioProcessor::signedBytesToIntArray(const std::string& bytes, int32_t bytesCount) {
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


Stream::Stream(const std::string& filePath) : path(filePath) {
	std::ofstream tmp;
	this->file.open(filePath, std::ios::binary);

	if (!this->file.is_open()) {
		throw Exception(std::string("Stream::Stream"), std::string("Invalid argument"), std::string("Can't open ") + this->path + " file.");
	}
}


void Stream::open() {
	this->file.open(this->path);

	if (!this->file.is_open()) {
		throw Exception(std::string("Stream::open"), std::string("Invalid argument"), std::string("Can't open ") + this->path + " file.");
	}
}


Stream::~Stream() {
	if (this->file.is_open()) {
		this->file.close();
	}
}


void Stream::read(int32_t size) {
	char charBuff[sizeof(size_t) * 8];

	if (size > sizeof(size_t) * 8) {
		throw Exception(std::string("Stream::open"), std::string("Invalid argument"), std::string("Can't read ") + std::to_string(size) + std::string(" bytes, maximum for this system is: ") + std::to_string(sizeof(size_t) * 8) + ".");
	}

	this->file.read(charBuff, size);

	this->readBuff.assign(charBuff, size);
}


bool Stream::isFileGood() const noexcept {
	return this->file.good();
}


const std::string& Stream::getPath() const noexcept {
	return this->path;
}


const std::string& Stream::getReadBuff() const noexcept {
	return this->readBuff;
}


void Stream::setPos(int32_t pos) {
	file.seekg(pos, std::ios_base::beg);
}


AudioStream::AudioStream(std::string filePath) : Stream(filePath) {
	std::ifstream headerFormatFile("./format.txt");
	static std::string templ = std::string("\u0052\u0049\u0046\u0046\u00E0\u0018\u004E\u0001\u0057\u0041\u0056\u0045\u0066\u006D\u0074\u0020\u0010\u0000\u0000\u0000\u0001\u0000\u0001\u0000\u0044\u00AC\u0000\u0000\u0088\u0058\u0001\u0000\u0002\u0000\u0010\u0000", 36); // header template, needed for file validation.
	int32_t bytesToSkip;

	bytesToSkip = 0;

	read(36);

	if (this->readBuff.substr(0, 4) != templ.substr(0, 4)) {
		throw Exception(std::string("AudioStream::AudioStream"), std::string("InvalidArgument"), std::string("File header is invalid, must be RIFF."));
	}
	else if (this->readBuff.substr(8, 4) != templ.substr(8, 4)) {
		throw Exception(std::string("AudioStream::AudioStream"), std::string("InvalidArgument"), std::string("File header is invalid, must be WAVE."));
	}
	else if (this->readBuff.substr(12, 4) != templ.substr(12, 4)) {
		throw Exception(std::string("AudioStream::AudioStream"), std::string("InvalidArgument"), std::string("File header is invalid, must be fmt."));
	}
	else if (this->readBuff.substr(16, 1) != templ.substr(16, 1)) {
		throw Exception(std::string("AudioStream::AudioStream"), std::string("InvalidArgument"), std::string("File header is invalid, block size is invalid."));
	}
	else if (this->readBuff.substr(20, 1) != templ.substr(20, 1)) {
		throw Exception(std::string("AudioStream::AudioStream"), std::string("InvalidArgument"), std::string("Wrong audio format (Must be PCM)."));
	}
	else if (this->readBuff.substr(22, 1) != templ.substr(22, 1)) {
		throw Exception(std::string("AudioStream::AudioStream"), std::string("InvalidArgument"), std::string("Wrong number of channels (Must be mono)."));
	}
	else if (this->readBuff.substr(24, 4) != templ.substr(24, 4)) {
		throw Exception(std::string("AudioStream::AudioStream"), std::string("InvalidArgument"), std::string("Wrong frequency (Must be 44100 hz)."));
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


const int32_t& AudioStream::getBytesPerSec() const noexcept {
	return this->bytesPerSec;
}


const int32_t& AudioStream::getBytesTillData() const noexcept {
	return this->bytesTillData;
}


const int32_t& AudioStream::getFileSize() const noexcept {
	return fileSize;
}


const int32_t& AudioStream::getDuration() const noexcept {
	return this->duration;
}


const int32_t& AudioStream::getBitsPerSample() const noexcept {
	return this->bitsPerSample;
}


const int32_t& AudioStream::getFrequency() const noexcept {
	return this->frequency;
}


void Processor::write(const std::string& string, int32_t size) {
	this->outputFile.write(string.c_str(), size);
}


void Processor::dumpOriginal(int32_t seconds) {
	for (size_t i = 0; i < seconds; i++) {
		for (size_t j = 0; j < this->stream.getBytesPerSec() / BuffSize; j++) {
			this->stream.read(BuffSize);
			write(this->stream.getReadBuff(), BuffSize);
		}
	}
}


Processor::Processor(AudioStream& stream, std::string& path, int32_t startPoint, int32_t endPoint) : stream(stream), outputPath(path) {
	this->outputFile.open(this->outputPath, std::ios_base::binary);

	if (!this->outputFile.is_open()) {
		throw Exception(std::string("Processor::Processor"), std::string("Invalid Argument"), std::string("Can't create ") + this->outputPath + std::string(" file."));
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


Processor::~Processor() {
	if (this->outputFile.is_open()) {
		this->outputFile.close();
	}
}


void Processor::apply() {
	throw Exception(std::string("Processor::apply"), std::string("Runtime error"), std::string("Base processor class can't do anything with the file."));
}


SaveProcessor::SaveProcessor(AudioStream& stream, std::string& path) : Processor(stream, path, 0, 0) {

}


void SaveProcessor::apply() {
	dumpOriginal(stream.getDuration());
}


MuteProcessor::MuteProcessor(AudioStream& stream, std::string& path, int32_t startPoint, int32_t endPoint) : Processor(stream, path, startPoint, endPoint) {

}


void MuteProcessor::apply() {
	std::string buff;

	buff.reserve(BuffSize);
	for (size_t i = 0; i < BuffSize; i++) {
		buff.push_back('\u0000');
	}

	this->stream.setPos(this->stream.getBytesTillData());

	if (this->end < this->start && this->end != -1) {
		throw Exception(std::string("MuteProcessor::apply"), std::string("Invalid argument"), std::string("Invalid end point, end point must be bigger than start point."));
	}

	if (this->end > this->stream.getDuration()) {
		throw Exception(std::string("MuteProcessor::apply"), std::string("Invalid argument"), std::string("Invalid end point, input file duration is smaller."));
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


const std::string MixProcessor::calculateMix(const std::string& bytes1, const std::string& bytes2) const noexcept {
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
			tmp = (signedBytesToInt(bytes1, bytes, static_cast<int32_t>(i) * bytes) + signedBytesToInt(bytes2, bytes, static_cast<int32_t>(i) * bytes)) / 2;
			res.push_back(static_cast<int8_t>(tmp & 0xFF));
			for (size_t j = 0; j < (this->stream.getBitsPerSample() / 8) - 1; j++) {
				res.push_back(static_cast<int8_t>((tmp >> 8) & 0xFF));
			}
		}
	}

	return std::as_const(res);
}


MixProcessor::MixProcessor(AudioStream& stream1, AudioStream& stream2, std::string& path, int32_t startPoint, int32_t endPoint) : Processor(stream1, path, startPoint, endPoint), secondStream(stream2) {

}


void MixProcessor::apply() {
	this->stream.setPos(this->stream.getBytesTillData() + 8);
	this->secondStream.setPos(this->secondStream.getBytesTillData() + 8);

	if (this->end < this->start && this->end != -1) {
		throw Exception(std::string("MixProcessor::apply"), std::string("Invalid argument"), std::string("Invalid end point, end point must be bigger than start point."));
	}

	if (this->end > this->stream.getDuration()) {
		throw Exception(std::string("MixProcessor::apply"), std::string("Invalid argument"), std::string("Invalid end point, input file duration is smaller."));
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


FrequencyProcessor::complexNum::complexNum(float r, float i) {
	this->real = r;
	this->imaginary = i;
}


FrequencyProcessor::complexNum FrequencyProcessor::complexNum::operator+(complexNum& b) {
	complexNum res;

	res.real = this->real + b.real;
	res.imaginary = this->imaginary + b.imaginary;

	return res;
}


FrequencyProcessor::complexNum FrequencyProcessor::complexNum::operator-(complexNum& b) {
	complexNum res;

	res.real = this->real - b.real;
	res.imaginary = this->imaginary - b.imaginary;

	return res;
}


FrequencyProcessor::complexNum FrequencyProcessor::complexNum::operator*(complexNum& b) {
	complexNum res;

	res.real = this->real * b.real - this->imaginary * b.imaginary;
	res.imaginary = this->real * b.imaginary + b.real * this->imaginary;

	return res;
}


void FrequencyProcessor::complexNum::operator*=(float b) {
	this->real = this->real * b;
	this->imaginary = this->imaginary * b;
}


std::vector<FrequencyProcessor::complexNum> FrequencyProcessor::fft(std::vector<complexNum>& signal, bool invert) {
	std::vector<complexNum> even, odd, evenRes, oddRes, res;
	complexNum omega, omegan;

	if (signal.size() == 1) {
		return signal;
	}

	for (size_t i = 0; i < signal.size() / 2; i++) {
		even.push_back(signal[i * 2]);
		odd.push_back(signal[i * 2 + 1]);
	}

	omega = complexNum(1.0f, 1.0f);
	omegan = complexNum(static_cast<float>(cos(2 * PI / signal.size() * (invert ? -1 : 1))), static_cast<float>(sin(2 * PI / signal.size() * (invert ? -1 : 1))));



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


FrequencyProcessor::FrequencyProcessor(AudioStream& stream, std::string& path, int32_t startFrequency, int32_t endFrequnecy) : Processor(stream, path, startFrequency, endFrequnecy) {

}


void FrequencyProcessor::apply() {
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


const std::string EchoProcessor::calculateMix(const std::string& bytes1, const std::string& bytes2) const noexcept {
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
			tmp = (signedBytesToInt(bytes1, bytes, static_cast<int32_t>(i) * bytes) + signedBytesToInt(bytes2, bytes, static_cast<int32_t>(i) * bytes)) / 2;
			res.push_back(static_cast<int8_t>(tmp & 0xFF));
			for (size_t j = 0; j < (this->stream.getBitsPerSample() / 8) - 1; j++) {
				res.push_back(static_cast<int8_t>((tmp >> 8) & 0xFF));
			}
		}
	}

	return std::as_const(res);
}


EchoProcessor::EchoProcessor(AudioStream& stream1, std::string& path, int32_t startPoint, int32_t endPoint) : Processor(stream1, path, startPoint, endPoint) {

}


void EchoProcessor::apply() {
	std::string tmp;

	this->stream.setPos(this->stream.getBytesTillData() + 8);

	if (this->end < this->start && this->end != -1) {
		throw Exception(std::string("EchoProcessor::apply"), std::string("Invalid argument"), std::string("Invalid end point, end point must be bigger than start point."));
	}

	if (this->end > this->stream.getDuration()) {
		throw Exception(std::string("EchoProcessor::apply"), std::string("Invalid argument"), std::string("Invalid end point, input file duration is smaller."));
	}

	dumpOriginal(this->start);


	if (this->end == -1) {
		for (size_t i = 0; i < this->stream.getDuration() - this->start; i++) {
			for (size_t j = 0; j < this->stream.getBytesPerSec() / BuffSize; j++) {
				this->stream.setPos(this->stream.getBytesTillData() + 8 + (static_cast<int32_t>(i) + this->start) * this->stream.getBytesPerSec() + static_cast<int32_t>(j) * BuffSize);
				this->stream.read(BuffSize);
				tmp = std::string(this->stream.getReadBuff());
				this->stream.setPos(this->stream.getBytesTillData() + 8 + static_cast<int32_t>(i) * this->stream.getBytesPerSec() + static_cast<int32_t>(j) * BuffSize);
				this->stream.read(BuffSize);

				write(calculateMix(tmp, this->stream.getReadBuff()), BuffSize);
			}
		}
	}
	else {
		for (size_t i = 0; i < this->end - this->start; i++) {
			for (size_t j = 0; j < this->stream.getBytesPerSec() / BuffSize; j++) {
				this->stream.setPos(this->stream.getBytesTillData() + 8 + (static_cast<int32_t>(i) + this->start) * this->stream.getBytesPerSec() + static_cast<int32_t>(j) * BuffSize);
				this->stream.read(BuffSize);
				tmp = std::string(this->stream.getReadBuff());
				this->stream.setPos(this->stream.getBytesTillData() + 8 + static_cast<int32_t>(i) * this->stream.getBytesPerSec() + static_cast<int32_t>(j) * BuffSize);
				this->stream.read(BuffSize);

				write(calculateMix(tmp, this->stream.getReadBuff()), BuffSize);
			}
		}

		dumpOriginal(this->stream.getDuration() - this->end);
	}
}


std::unique_ptr<Processor> Factory::createProcessor(std::vector<std::shared_ptr<AudioStream>>& streams, std::string& outputPath, int32_t startPoint, int32_t endPoint) {
	std::unique_ptr<Processor> res(new Processor(*streams[0].get(), outputPath, startPoint, endPoint));

	return res;
}


std::unique_ptr<Processor> SaveFactory::createProcessor(std::vector<std::shared_ptr<AudioStream>>& streams, std::string& outputPath, int32_t startPoint, int32_t endPoint) {
	std::unique_ptr<Processor> res(new SaveProcessor(*streams[0].get(), outputPath));

	return res;
}


std::unique_ptr<Processor> MuteFactory::createProcessor(std::vector<std::shared_ptr<AudioStream>>& streams, std::string& outputPath, int32_t startPoint, int32_t endPoint) {
	std::unique_ptr<Processor> res(new MuteProcessor(*streams[0].get(), outputPath, startPoint, endPoint));

	return res;
}


std::unique_ptr<Processor> MixFactory::createProcessor(std::vector<std::shared_ptr<AudioStream>>& streams, std::string& outputPath, int32_t startPoint, int32_t endPoint) {
	std::unique_ptr<Processor> res(new MixProcessor(*streams[0].get(), *streams[1].get(), outputPath, startPoint, endPoint));

	return res;
}


std::unique_ptr<Processor> FftFactory::createProcessor(std::vector<std::shared_ptr<AudioStream>>& streams, std::string& outputPath, int32_t startPoint, int32_t endPoint) {
	std::unique_ptr<Processor> res(new FrequencyProcessor(*streams[0].get(), outputPath, startPoint, endPoint));

	return res;
}


std::unique_ptr<Processor> EchoFactory::createProcessor(std::vector<std::shared_ptr<AudioStream>>& streams, std::string& outputPath, int32_t startPoint, int32_t endPoint) {
	std::unique_ptr<Processor> res(new EchoProcessor(*streams[0].get(), outputPath, startPoint, endPoint));

	return res;
}


ConfigHandler::commandStruct ConfigHandler::getLineInput(std::string& line, int32_t lineIdx) {
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
		throw Exception(std::string("ConfigHandler::commandStruct"), std::string("Invalid argument"), std::string("Not enough arguments provided in line: ") + std::to_string(lineIdx) + '.');
	}
	if (links.size() == 0 && res.commandName == std::string("mix")) {
		throw Exception(std::string("ConfigHandler::commandStruct"), std::string("Invalid argument"), std::string("File to mix with isn't provided in line: ") + std::to_string(lineIdx) + '.');
	}
	for (size_t i = 0; i < links.size(); i++) {
		if (links[i] > inputs.size()) {
			throw Exception(std::string("ConfigHandler::commandStruct"), std::string("Invalid argument"), std::string("Link to file is out of bounds in line: ") + std::to_string(lineIdx) + '.');
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


std::vector<std::reference_wrapper<const std::string>> ConfigHandler::getMapKeys(std::map<std::string, Factory*>& mapIn) {
	std::vector<std::reference_wrapper<const std::string>> res;

	for (auto iter = mapIn.begin(); iter != mapIn.end(); iter++) {
		res.push_back(mapIn.find(iter->first)->first);
	}

	return res;
}


void ConfigHandler::save(std::string& path, std::map<std::string, Factory*>& mapIn) {
	std::vector<std::shared_ptr<AudioStream>> streams;

	std::shared_ptr<AudioStream> finalStream = std::make_shared<AudioStream>(path);

	streams.push_back(finalStream);

	mapIn["save"]->createProcessor(streams, this->outputPath, 0, 0)->apply();
}


ConfigHandler::ConfigHandler(std::string& outputPath, std::string& configPath, std::vector<std::shared_ptr<std::string>>& inputsVec) : path(configPath), outputPath(outputPath), inputs(inputsVec) {
	file.open(this->path);

	if (!file.is_open()) {
		throw Exception(std::string("ConfigHandler::ConfigHandler"), std::string("Invalid argument"), std::string("Can't open ") + this->path + std::string(" file.\n"));
	}
}


ConfigHandler::~ConfigHandler() {
	if (this->file.is_open()) {
		file.close();
	}
}


void ConfigHandler::handleConfig() {
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
				throw Exception(std::string("ConfigHandler::handleConfig"), std::string("Invalid argument"), std::string("Config is invalid, error occured in line: ") + std::to_string(lineIndex) + std::string(". Consider using a -h flag when launching the programm."));
			}
		}

		lineIndex++;

		if (procIndex > 1) {
			std::remove((outputPath.substr(0, outputPath.find(".wav")) + std::to_string(procIndex - 2) + std::string(".tmp")).c_str());
		}
	}

	//for (size_t i = 0; i < keys.size(); i++) {
	//	delete factories[keys[i]];
	//}

	save(curPath, factories);

	std::remove(curPath.c_str());
}