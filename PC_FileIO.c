/**********************************************************************
FILE IO C FILE
**********************************************************************/


#pragma SystemFile
#ifndef PC_FILEIO
#define PC_FILEIO

// if platform is not specified, default is EV3
#ifndef _EV3FILEIO
#define _EV3FILEIO 1
#endif

#define _MAIN_PCFILEIO 0  // 1 for test main program, 0 for use in other programs

// ASCII characters
const byte _NULL = 0;
const byte _TAB = 9;
const byte _LF = 10;
const byte _CR = 13;

// define equivalents for EV3 and NXT platforms
#if _EV3FILEIO
typedef int TFileHandle;
typedef bool TFileIOResult;
#endif

// status flag
#if _EV3FILEIO
const TFileIOResult _IO_OKAY = true;
#else
const TFileIOResult _IO_OKAY = (TFileIOResult)0;
#endif

bool openWritePC(TFileHandle & fout, char* name, word fileSize=1000)
{
    TFileIOResult status = _IO_OKAY;
#if _EV3FILEIO
    fout = fileOpenWrite(name);
#else
    Delete(name, status);
    OpenWrite(fout, status, name, fileSize);
#endif
    return status == _IO_OKAY;
}

bool openReadPC(TFileHandle & fin, char* name)
{
    TFileIOResult status = _IO_OKAY;
#if _EV3FILEIO
    fin = fileOpenRead(name);
#else
    word fileSize = 0;
    OpenRead(fin, status, name, fileSize);
#endif
    return status == _IO_OKAY;
}

bool closeFilePC(TFileHandle & fileHandle)
{
    TFileIOResult status = _IO_OKAY;
#if _EV3FILEIO
    status = fileClose(fileHandle);
#else
    Close(fileHandle, status);
#endif
    return status == _IO_OKAY;
}

bool writeCharPC(TFileHandle & fout, byte charmsg)
{
    TFileIOResult status = _IO_OKAY;
#if _EV3FILEIO
    status = fileWriteChar(fout, charmsg);
#else
    WriteByte(fout, status, charmsg);
#endif
    return status == _IO_OKAY;
}

bool writeEndlPC(TFileHandle & fout)
{
    bool statusOK = writeCharPC(fout, _CR);  // both CR and LF are needed for simple editors (e.g. Notepad)
    if (statusOK)
        statusOK = writeCharPC(fout, _LF);
    return statusOK;
}

bool writeTextPC(TFileHandle & fout, char* textmsg)
{
    TFileIOResult status = _IO_OKAY;
#if _EV3FILEIO
//    int msglen = strlen(textmsg) + 1;  // add 1 to include the zero terminator
//    status = fileWriteData(fout, textmsg, msglen);
    int msglen = strlen(textmsg);
    for (int charCount = 0; charCount < msglen; charCount++)
        writeCharPC(fout, textmsg[charCount]);
#else
    WriteText(fout, status, textmsg);
#endif
    return status == _IO_OKAY;
}

bool writeLongPC(TFileHandle & fout, long number)
{
    string outputString;
    stringFormat(outputString, "%d", number);
    return writeTextPC(fout, outputString);
}

bool writeFloatPC(TFileHandle & fout, string const & numFormat, float number)
{
    string outputString;
    stringFormat(outputString, numFormat, number);
    return writeTextPC(fout, outputString);
}

bool writeFloatPC(TFileHandle & fout, float number)
{
    return writeFloatPC(fout, "%f", number);
}

bool readBytePC(TFileHandle & fin, byte & charmsg)
{
    TFileIOResult status = _IO_OKAY;
#if _EV3FILEIO
    status = fileReadChar(fin, &charmsg);
#else
    ReadByte(fin,status, charmsg);
#endif
    return status == _IO_OKAY;
}

bool _isWhiteSpace(char c)
{
    return c == _NULL || c == ' ' || c == _TAB || c == _CR || c == _LF;
}

bool readCharPC(TFileHandle & fin, char & charmsg)
{
    // ignore leading whitespace or NULLs
    bool status = true;
    do
        status = readBytePC(fin, charmsg);
    while (status && _isWhiteSpace(charmsg));
    return status;
}

bool readTextPC(TFileHandle & fin, string & result) // a maximum of 20 characters can be read
{
    const int MAX_SIZE = 20;
    byte msg[MAX_SIZE+1];  // add one to size so can be null terminated

    // ignore leading whitespace or NULLs
    bool statusChar = readCharPC(fin, msg[0]);

    // get string, first byte is already available from above if statusChar is true
    int n = 0;
    while(statusChar && n < MAX_SIZE && !_isWhiteSpace(msg[n])) // characters will be lost if string size exceeds 20
    {
        n++;
        statusChar = readBytePC(fin, msg[n]);
    }

    bool status = true;
    if (n > 0)
    {
        msg[n] = _NULL;  // null terminate the string
        stringFromChars(result, &msg[0]);
        status = true;
    }
    else
    {
        result = "";
        status = false;
    }
    return status;
}

bool readIntPC(TFileHandle & fin, int & number)
{
    string numstr;
    bool status = readTextPC(fin, numstr);
    if (status)
        number = atoi(numstr);
    return status;
}

bool readFloatPC(TFileHandle & fin, float & number)
{
    string numstr;
    bool status = readTextPC(fin, numstr);
    if (status)
        number = atof(numstr);
    return status;
}

#if _MAIN_PCFILEIO
task main()
{
    TFileHandle fout;
    word fileSize = 1000;
    bool fileOkay = openWritePC(fout, "fileWrite.txt", fileSize);
    if (!fileOkay)
    {
        displayString(0,"Outfile error");
        wait1Msec (5000);
    }
    else
    {
        writeCharPC(fout, 'A');  // will ignore status for now
        writeEndlPC(fout);
        writeCharPC(fout, 'B');
        writeEndlPC(fout);
        writeTextPC(fout, "Hello");
        writeEndlPC(fout);
        writeCharPC(fout, '4');
        writeCharPC(fout, '2');
        writeEndlPC(fout);
        writeLongPC(fout, 5000);
        writeCharPC(fout, ' ');
        writeFloatPC(fout, PI);
        writeCharPC(fout, ' ');
        writeFloatPC(fout, "%.2f", 6.789);
        writeEndlPC(fout);
        for (long i = 1; i <= 1000000000; i = i * 10)
        {
            writeLongPC(fout, i-1);
            writeCharPC(fout, ' ');
            writeLongPC(fout, i);
            writeCharPC(fout, ' ');
            writeLongPC(fout, i+1);
            writeEndlPC(fout);
            writeLongPC(fout, -(i-1));
            writeCharPC(fout, ' ');
            writeLongPC(fout, -i);
            writeCharPC(fout, ' ');
            writeLongPC(fout, -(i+1));
            writeEndlPC(fout);
        }
        if (closeFilePC(fout))
            displayString(0,"Write file closed okay");
        else
            displayString(0,"Write file error");
        wait1Msec(5000);
    }
/* Output file should be:
A
B
Hello
42
5000  3.141593  6.79
0  1  2
0  -1  -2
9  10  11
-9  -10  -11
99  100  101
-99  -100  -101
999  1000  1001
-999  -1000  -1001
9999  10000  10001
-9999  -10000  -10001
99999  100000  100001
-99999  -100000  -100001
999999  1000000  1000001
-999999  -1000000  -1000001
9999999  10000000  10000001
-9999999  -10000000  -10000001
99999999  100000000  100000001
-99999999  -100000000  -100000001
999999999  1000000000  1000000001
-999999999  -1000000000  -1000000001
*/


/* Test file for read contains (with no whitespace after last character):
A B   C GENE_121
-425 3.6
-35.25

omega
*/
    eraseDisplay();
    TFileHandle fin;
    fileOkay = openReadPC(fin, "fileRead.txt");
    if (!fileOkay)
    {
        displayString(0,"Infile error");
        wait1Msec (5000);
    }
    else
    {
        char letter;
        readCharPC(fin,letter);
        displayString(0,"%c",letter);
        readCharPC(fin,letter);
        displayString(1,"%c",letter);
        readCharPC(fin,letter);
        displayString(2,"%c",letter);
        string phrase;
        readTextPC(fin,phrase);
        displayString(3,"%s",phrase);
        int numint;
        readIntPC(fin,numint);
        displayString(4,"%d",numint);
        float numfloat;
        readFloatPC(fin,numfloat);
        displayString(5,"%f",numfloat);
        readFloatPC(fin,numfloat);
        displayString(6,"%f",numfloat);
        readTextPC(fin,phrase);
        displayString(7,"%s",phrase);
        wait1Msec(20000);

        if (closeFilePC(fin))
            displayString(0,"Read file closed okay");
        else
            displayString(0,"Read file error");
        wait1Msec(5000);
    }
}
#endif

#endif
/**********************************************************************
Copyright(c) 2015-2016 C.C.W. Hulls, P.Eng.
Students, staff, and faculty members at the University of Waterloo
are granted a non-exclusive right to copy, modify, or use this
software for non-commercial teaching, learning, and research purposes
provided the author(s) are acknowledged except for as noted below.
**********************************************************************/
