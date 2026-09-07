#ifndef HEADERS_FUNCTIONS_OVERLAY30_H_
#define HEADERS_FUNCTIONS_OVERLAY30_H_

void WriteQuicksaveData(void* buffer, int buffer_size);
void InitDataSerializerWriter(struct data_serializer* serializer, uint8_t* stream, int num_bytes);
void WriteBitsToDataSerializer(struct data_serializer* serializer, void* src, int num_bits);
void InitDataSerializerReader(struct data_serializer* serializer, uint8_t* stream, int num_bytes);
void ReadBitsFromDataSerializer(struct data_serializer* serializer, void* buffer, int num_bits);
void FinishDataSerializer(struct data_serializer* serializer);

#endif
