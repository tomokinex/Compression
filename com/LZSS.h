//ASCII8bit(1bit+7bit)を対象
//圧縮すると16bit(1bit+12bit+3bit)valid,windowsize,lengthの順
//lengthは必ず2以上になるので, +2した値を用いる(ex 000はlength:2)

//windowの再起参照は実装していない
#define windowsize_bit 12
#define length_bit 3
#define windowsize (1 << windowsize_bit)
#define length (1 << length_bit) + 2
#define c_bits (1 + windowsize_bit + length_bit)
#define length_mask (1 << length_bit) - 1
#define windowsize_mask ( (1 << windowsize_bit) - 1 ) << length_bit
#define c_bits_mask 1 << (c_bits - 1)

#include<vector>
#include<math.h>
#include<assert.h>
#include<iostream>
namespace LZSS{
bool decompress(unsigned char s1, unsigned char s2, int *head, int *len){
    //圧縮bit
    if((s1 >> 7) == 1){
        short comp = ((short)s1 << 8) | (short)s2;
        *len = (int)(comp & length_mask) + 2;
        *head = (int)((comp & windowsize_mask) >> length_bit);
        std::cout << std::hex << std::showbase << (int)s1 << " " << (int)s2 << std::endl;
        std::cout << "h: " << *head << " l: " << *len << std::endl;
        return true;
    }else{
      return false;          
    }
}
void compress(int head, int len, unsigned char* s1, unsigned char* s2){
    short comp = c_bits_mask;
    assert(len >= 2);
    head <<= length_bit;
    comp |= head;
    comp |= (len-2);
    //std::cout << std::hex<< std::showbase<<"h : " << (head >> length_bit) << "len : " << len << "comp " << comp << std::endl; 
    *s2 = (unsigned char)(comp & ((1<<8)-1));
    *s1 = (unsigned char)(comp >> 8);
}

class ring_buffer{
    public:
    int m_size, m_head, m_cov;
    std::vector<unsigned char> v;
    ring_buffer(int size){
        m_size = size;
        v.resize(size);
        m_head = 0;
        m_cov = 0;
    }
    ~ring_buffer(){
        v.resize(0);
    }
    int Ishead(){
        return m_head;
    }
    int Iscov(){
        return m_cov;
    }
    int Isbottom(){
        if(Isfilling()){
            return (Ishead()+1)%m_size;
        }else{
            return 0;
        }
    }
    void head_inc(){
        m_head++;
        if(m_head >= m_size){
            m_head -= m_size;
        }
    }
    void conv_inc(){
        m_cov++;
        if(m_cov > m_size){
            m_cov = m_size;
        }    
    }
    bool Isfilling(){
        return !(m_cov < m_size);
    }
    void push(unsigned char value){
        v[m_head] = value;
        head_inc();
        conv_inc();
    }
    unsigned char pop(int iterator){
        return v[iterator%m_size];
    }

    //windowのheadとfileのhead固定
    int matching(std::vector<unsigned char>::iterator s, int w_head_init, int max_num){
        int w_head = w_head_init;
        int len = 0;
        while(len <= max_num){
            //std::cout << "pop[" << w_head << "] : " <<(int)pop(w_head) << " s+" << len << " : " << (int)(*s+len) << std::endl;             
            if(pop(w_head) != *(s+len)){
                break;
            }else{
                w_head++;
                len++;            
            }
        }
        //std::cout << "len: " << len << std::endl;
        return len;
    }

    bool matching_cycle(std::vector<unsigned char>::iterator s, int left_num,int*head, int*len){
        int max_len = 0;
        int w_head = 0;

        for(int i=0;i<m_cov;i++){
            int t_len = matching(s, Isbottom()+i, std::min(m_cov-i, left_num));
            if(t_len > max_len){
                max_len = t_len;
                w_head = i;
            }
        }
        if(max_len >= 2){
            *head = w_head;
            *len = max_len;
            return true;
        }else{
            return false;
        }
    }

    };
    void LZSS_compress(std::vector<unsigned char>& file, std::vector<unsigned char>& comp){
        ring_buffer m_ring(windowsize);
	    auto file_pointer = file.begin();
	    int file_size = file.size();		
	    while(file_size > 0){
		    int head = 0;
		    int len = 0;
		    //圧縮成功
		    if(m_ring.matching_cycle(file_pointer,file_size-1,&head, &len)){
			    unsigned char s1 = 0;
			    unsigned char s2 = 0;
			    compress(m_ring.Iscov()-head-1, len, &s1, &s2);
			    comp.push_back(s1);
			    comp.push_back(s2);
			    for(auto j=0;j<len;j++){
				    m_ring.push(*file_pointer);
				    file_pointer++;
				    file_size--;
			    }
		    //失敗
		    }else{
			    comp.push_back(*file_pointer);
			    m_ring.push(*file_pointer);
			    file_pointer++;
			    file_size--;
		    }
	    }
    }
    void LZSS_decompress(std::vector<unsigned char>& in, std::vector<unsigned char>& decomp){
        int in_size = in.size();
        for(int i=0;i<in_size-1;i++){
            unsigned char s1 = in[i];
            unsigned char s2 = in[i+1];
            int head = 0;
            int len = 0;
            if(decompress(s1,s2,&head,&len)){
                for(int j=0;j<len;j++){
                    int t_decomp_size = decomp.size();
                    decomp.push_back(decomp[t_decomp_size-1-head+j]);
                }
                i++;
            }else{
                decomp.push_back(s1);
                if(i == in_size-2){
                    decomp.push_back(s2);
                }
            }
        }
    }

}