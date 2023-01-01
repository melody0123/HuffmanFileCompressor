#ifndef COMPRESSION_H_INCLUDED
#define COMPRESSION_H_INCLUDED

#include "huff_tree.h"


struct buffer
{
    unsigned char ch; //存储字符
    unsigned int bits; //缓冲区中的比特数
};

class HuffCompression
{
private:
    FILE *infp, *outfp; //输入输出文件流
    buffer buf; //缓冲区

    //辅助函数
    void write_bit(unsigned int bit); //向目标文件写入 1 bit
    void enforce_write_bits(); //强制把缓冲区中的数据写入文件，用于文件结尾

public:
    HuffCompression(){} //构造函数
    ~HuffCompression(){} //析构函数
    void compress(); //压缩操作
    void decompress(); //解压操作
};

void HuffCompression::write_bit(unsigned int bit)
{
    buf.bits++; //计数器加一
    buf.ch = (buf.ch << 1) | bit; //把 1 bit 写入缓冲区
    if(buf.bits == 8)
    {
        //缓冲区已满
        fputc(buf.ch, outfp); //写入文件
        buf.bits = 0; //计数器清零
        buf.ch = 0; //字符清零
    }
    return;
}

void HuffCompression::enforce_write_bits()
{
    unsigned int len = buf.bits;
    if(len > 0) //缓冲区不空
    {
        for(unsigned int i = 0; i < 8 - len; i++)
            write_bit(0);
    }
    return;
}

void HuffCompression::compress()
{
    char in_fname[256], out_fname[256]; //输入输出文件名

    printf("请输入源文件名（文件不得超过 4 GB）："); //打开文件
    scanf("%s", in_fname);
    if((infp = fopen(in_fname, "rb")) == nullptr)
    {
        printf("文件打开失败！\n");
        return;
    }

    fgetc(infp); //试探性读一个字符
    if(feof(infp))
    {
        printf("空的文件！\n");
        return;
    }

    printf("请输入目标文件名："); //打开文件
    scanf("%s", out_fname);
    if((outfp = fopen(out_fname, "wb")) == nullptr)
    {
        printf("文件打开失败！\n");
        return;
    }

    printf("开始压缩，请稍后...\n");

    const unsigned long n = 256; //字符种类数
    unsigned char ch[n]; //字符数组
    unsigned long w[n]; //将字符出现的次数作为它的权重
    unsigned long size = 0; //文件大小（字节数），有了这个变量就可以不用管编码后末尾多出来几位

    for(int i = 0; i < n; i++) //初始化
    {
        ch[i] = i;
        w[i] = 0;
    }

    rewind(infp);
    char c = fgetc(infp);
    while(!feof(infp))
    {
        w[(unsigned char)c]++; //统计字符出现次数
        size++; //文件大小自增
        c = fgetc(infp); //取出下一个字符
    }

    HuffmanTree<unsigned char, unsigned long> *p_tree = new HuffmanTree<unsigned char, unsigned long>(ch, w, n);

    rewind(outfp); //向目标文件写入
    fwrite(&size, sizeof(unsigned long), 1, outfp); //写入源文件大小
    for(int i = 0; i < n; i++)
    {
        fwrite(&w[i], sizeof(unsigned long), 1, outfp);
    }

    buf.bits = 0;
    buf.ch = 0; //初始化
    rewind(infp); //文件位置指针复位
    c = fgetc(infp); //试探性取一个字符
    while(!feof(infp))
    {
        string tmp = p_tree->encode(c); //查找c的编码
        for(int i = 0; i < tmp.length(); i++) //写入缓冲区
        {
            if(tmp[i] == '0') write_bit(0);
            else write_bit(1);
        }
        c = fgetc(infp); //再次读取一个字节
    }
    enforce_write_bits(); //清空缓冲区

    fclose(infp);
    fclose(outfp);

    printf("处理完毕！\n");
}

void HuffCompression::decompress()
{
    char in_fname[256], out_fname[256]; //输入输出文件名

    printf("请输入源文件名："); //打开文件
    scanf("%s", in_fname);
    if((infp = fopen(in_fname, "rb")) == nullptr)
    {
        printf("文件打开失败！\n");
        return;
    }

    fgetc(infp); //试探性读一个字符
    if(feof(infp))
    {
        printf("空的文件！\n");
        return;
    }

    printf("请输入目标文件名："); //打开文件
    scanf("%s", out_fname);
    if((outfp = fopen(out_fname, "wb")) == nullptr)
    {
        printf("文件打开失败！\n");
        return;
    }

    printf("开始解压缩，请稍后...\n");

    const unsigned long n = 256;
    unsigned char ch[n];
    unsigned long w[n];
    unsigned long size = 0; //意义同上

    rewind(infp); //文件位置指针复位
    fread(&size, sizeof(unsigned long), 1, infp); //写入目标文件大小

    for(int i = 0; i < n; i++) //初始化
    {
        ch[i] = (char)i;
        fread(&w[i], sizeof(unsigned long), 1, infp);
    }

    HuffmanTree<unsigned char, unsigned long> *p_tree = new HuffmanTree<unsigned char, unsigned long>(ch, w, n); //建树

    char c = fgetc(infp);
    string tmp;
    while(!feof(infp))
    {
        unsigned char uc = (unsigned char)c;
        for(int i = 0; i < 8; i++) //c 转换成二进制串
        {
            if(uc < 128) tmp = tmp + "0";
            else tmp = tmp + "1";
            uc << 1;
        }
        c = fgetc(infp);
    }

    LinkList<unsigned char> origin_file = p_tree->decode(tmp, size); //译码
    for(node<unsigned char> *cur_ptr = (origin_file.get_head())->next; cur_ptr != nullptr; cur_ptr = cur_ptr->next)
    {
        fputc(cur_ptr->data, outfp);
    }

    fclose(infp);
    fclose(outfp);

    printf("处理结束！\n");

    return;
}

#endif // COMPRESSION_H_INCLUDED
