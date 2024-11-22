/**
 * @file
 * @brief The API provides functions for communicating with the second CPU of a redundant CPU pair.
 *
 * This API provides functionality for synchronous and asynchronous data exchange 
 * between a redundant CPU pair connected via rLink.
 * The Data exchange is organized in communication channels with unique name identifier.
 * A communication channel is capable of sending and receiving asynchronous data on demand,
 * as well as copying a contiguous memory block synchronously with every cyclic taskclass execution.
 * 
 * @ingroup rcpu
 */
#ifndef _RLIB_H_
#define _RLIB_H_

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief The handle referring to a communication channel.
 */
typedef int32_t RLIB_H;

/**
 * @brief Open a communication channel for data transport in redundancy context.
 * @param name The unique name identifying the communication channel to be used on primary and secondary CPU.
 * @param size The payload size in bytes mandatory for synchronous data exchange (optional).
 * @param tx The memory address of producer (optional).
 * @param rx The memory address of consumer (optional).
 * 
 * If all optional parameters are set, the source data provided by the producer is copied
 * to the destination memory of the consumer on every cycle of the calling task class.
 * 
 * @returns The handle of the opened communication channel.
 */
RLIB_H rOpen(const char* name, uint32_t size, const void* tx, void* rx);

/**
 * @brief Close dedicated communication channel.
 * @param handle The handle of the communication channel.
 */
void rClose(RLIB_H handle);

/**
 * @brief Receive data by reading from the communication channel.
 * @param handle The handle of the communication channel.
 * @param buffer The destination buffer for the data to be read.
 * @param bytes The size in bytes of the destination buffer. 
 * 
 * @returns -1 on error, else the amount of bytes that could be read.
 */
int32_t rRead(RLIB_H handle, void* buffer, uint32_t bytes);

/**
 * @brief Send data by writing it to the communication channel.
 * @param handle The handle of the communication channel.
 * @param buffer The source buffer for data to be written.
 * @param bytes The size in bytes of the source buffer. 
 * 
 * @returns -1 on error, else the amount of bytes that could be written.
 */
int32_t rWrite(RLIB_H handle, const void* buffer, uint32_t bytes);

#ifdef __cplusplus
}
#endif

#endif
