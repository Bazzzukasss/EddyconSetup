/* 
 * File:   HWNetPacket.h
 * Author: Baza
 *
 * Created on March 20, 2015, 10:44 AM
 */

#ifndef HWNETPACKET_H
#define	HWNETPACKET_H
#include <string.h>
#include <iostream>
using namespace std;


typedef int hwpacket_t;

#define PACKET_HEADER_SIZE_B   44
#define MIN_PACKET_DATA_SIZE    1024
#define PACKET_DATA_SIZE_B     (16384)
#define PACKET_SIZE_B   (PACKET_DATA_SIZE_B+PACKET_HEADER_SIZE_B)
struct HWPacketHeader{
    hwpacket_t cmd;
    hwpacket_t res;
    hwpacket_t data_size_b;
    hwpacket_t param[(PACKET_HEADER_SIZE_B-3*sizeof(hwpacket_t))/4];
};
struct HWPacket{
    HWPacketHeader  header;
    hwpacket_t      data[PACKET_DATA_SIZE_B/4];
};
class HWNetPacket{
    public:
        HWNetPacket(){};
        ~HWNetPacket(){};
        inline HWPacket* getPacket(){return &packet;}
        inline HWPacketHeader* getHeader(){return &packet.header;}
        inline void setHeaderParamValue(int _num,int _value){packet.header.param[_num]=_value;}
        inline int getHeaderParamValue(int _num){return packet.header.param[_num];}
        inline hwpacket_t getCmd(){ return packet.header.cmd;}
        inline hwpacket_t getCmd(int _level){ return ((packet.header.cmd>>(_level*8))&0xFF);}
        inline hwpacket_t* getData(){return packet.data;}
        inline hwpacket_t getRes(){ return packet.header.res;}
        inline void setData(int _index,hwpacket_t _value){packet.data[_index]=_value;}
        inline void setRecivedSize(int _size_b){recivedSize_b=_size_b;}
        inline int getRecivedSize(){return recivedSize_b;}
        inline int getSendSize(){return (packet.header.data_size_b+PACKET_HEADER_SIZE_B);}
        inline int getSendDataSize(){return (packet.header.data_size_b);}
        void clear()
        {
            memset(&packet,0,PACKET_SIZE_B);
        }
        void clearHeader()
        {
            memset(&packet.header,0,PACKET_HEADER_SIZE_B);
        }
        void fill(int _cmd,int _res,int _data_size_b,hwpacket_t* _data)
        {
            packet.header.cmd=_cmd;
            packet.header.res=_res;
            if(_data_size_b>PACKET_DATA_SIZE_B)
            {
                _data_size_b=PACKET_DATA_SIZE_B;
                cout<<"Error:packet data size is to big!"<<endl;
            }
            packet.header.data_size_b=_data_size_b;
            if(_data)
                memcpy(packet.data,_data,_data_size_b);
            else
                memset(packet.data,0,PACKET_DATA_SIZE_B);
        }
        void fill(int _res,int _data_size_b)
        {
            packet.header.res=_res;
            if(_data_size_b>PACKET_DATA_SIZE_B)
            {
                _data_size_b=PACKET_DATA_SIZE_B;
                cout<<"Error:packet data size is to big!"<<endl;
            }
            packet.header.data_size_b=_data_size_b;
        }
        inline void fill(int _res){ fill(packet.header.cmd,_res,0,0);}
        inline void fill(int _res,int _data_size_b,hwpacket_t* _data){ fill(packet.header.cmd,_res,_data_size_b,_data);}
        inline void fill(HWPacket* _hwpacket){memcpy(&packet,_hwpacket,PACKET_SIZE_B);}
        inline void show(){ 
            cout<<dec<< "Packet:"<<endl;
            cout<<dec<<"\trecived size: "<<recivedSize_b<<" bytes"<<endl;
            cout<<hex<<"\tcmd: 0x"<<getCmd(0)<<" 0x"<<getCmd(1)<<" 0x"<<getCmd(2)<<" 0x"<<getCmd(3)<<endl;
            cout<<hex<<"\tres: 0x"<<packet.header.res<<endl;
            cout<<dec<<"\tdata size: "<<packet.header.data_size_b<<" bytes"<<endl;
        }
    private:
        HWPacket   packet;
        int recivedSize_b;
};


#endif	/* HWSERVERPACKET_H */

