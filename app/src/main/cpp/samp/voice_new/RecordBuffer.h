#pragma once

class RecordBuffer {
public:
    RecordBuffer(int bufferSize);
    ~RecordBuffer();

    short *getRecordBuffer();
    short *getNowBuffer();

public:
    short **buffer;
    int index = -1;
};