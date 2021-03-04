#ifndef DATA_MODULE_3RD_PARTY_PANDAS_PLINK_DATA_READER_H
#define DATA_MODULE_3RD_PARTY_PANDAS_PLINK_DATA_READER_H

int read_bed_chunk(char* filepath, uint64_t nrows, uint64_t ncols, uint64_t row_start, uint64_t col_start,
                   uint64_t row_end, uint64_t col_end, uint8_t* out, uint64_t* strides);


#endif // DATA_MODULE_3RD_PARTY_PANDAS_PLINK_DATA_READER_H
