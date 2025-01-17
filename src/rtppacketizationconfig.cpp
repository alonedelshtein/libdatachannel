/**
 * Copyright (c) 2020 Filip Klembara (in2core)
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
 */

#if RTC_ENABLE_MEDIA

#include "rtppacketizationconfig.hpp"

#include <cassert>
#include <limits>
#include <random>

namespace rtc {

RtpPacketizationConfig::RtpPacketizationConfig(SSRC ssrc, string cname, uint8_t payloadType,
                                               uint32_t clockRate, uint8_t videoOrientationId)
    : ssrc(ssrc), cname(cname), payloadType(payloadType), clockRate(clockRate),
      videoOrientationId(videoOrientationId) {
	assert(clockRate > 0);

	// RFC 3550: The initial value of the sequence number SHOULD be random (unpredictable) to make
	// known-plaintext attacks on encryption more difficult [...] The initial value of the timestamp
	// SHOULD be random, as for the sequence number.
	std::default_random_engine rng(std::random_device{}());
	std::uniform_int_distribution<uint32_t> dist(0, std::numeric_limits<uint32_t>::max());
	sequenceNumber = static_cast<uint16_t>(dist(rng));
	timestamp = startTimestamp = dist(rng);
}

double RtpPacketizationConfig::getSecondsFromTimestamp(uint32_t timestamp, uint32_t clockRate) {
	return double(timestamp) / double(clockRate);
}

double RtpPacketizationConfig::timestampToSeconds(uint32_t timestamp) {
	return RtpPacketizationConfig::getSecondsFromTimestamp(timestamp, clockRate);
}

uint32_t RtpPacketizationConfig::getTimestampFromSeconds(double seconds, uint32_t clockRate) {
	return uint32_t(int64_t(seconds * double(clockRate))); // convert to integer then cast to u32
}

uint32_t RtpPacketizationConfig::secondsToTimestamp(double seconds) {
	return RtpPacketizationConfig::getTimestampFromSeconds(seconds, clockRate);
}

void RtpPacketizationConfig::setStartTime(double startTime, EpochStart epochStart,
                                          optional<uint32_t> startTimestamp) {
	// Deprecated dummy function
	this->startTime = startTime + double(static_cast<uint64_t>(epochStart));
	if (startTimestamp.has_value())
		this->timestamp = this->startTimestamp = startTimestamp.value();
}

} // namespace rtc

#endif /* RTC_ENABLE_MEDIA */
