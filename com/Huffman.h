#include<vector>


namespace Huff{
    class tree{
        public:
        int m_num;
        unsigned char m_val;
        tree* m_parent;
        tree* m_left;
        tree* m_right;
        tree(){
            m_num = 0;
            m_val = 0;
            m_parent = nullptr;
            m_left = nullptr;
            m_right = nullptr;
        }
        void add(){
            m_num++;
        }
        bool Isparent(){
            return m_parent != nullptr;
        }
        int Isnum(){
            return m_num;
        }
    };

    void min_search(std::vector<tree> hist, int*min_index1, int*min_index2){
        int min1 = __INT_MAX__;
        int min2 = __INT_MAX__;
        int index1 = -1;
        int index2 = -1;
        for(int i=0;i<hist.size();i++){
            if(!hist[i].Isparent()){
                if(min1 > hist[i].Isnum()){
                    min2 = min1;
                    index2 = index1;
                    min1 = hist[i].Isnum();
                    index1 = i;
                }else{
                    if(min2 > hist[i].Isnum()){
                        min2 = hist[i].Isnum();
                        index2 = i;
                    }
                }
            }
        }
        *min_index1 = index1;
        *min_index2 = index2;
    }


    void Huff_compress(std::vector<unsigned char>& file, std::vector<unsigned char>& comp){
        std::vector<tree> m_tree(257);
        for(int i=0;i<file.size();i++){
            m_tree[(int)file[i]].add();
        }
    }
    void Huff_decompress(std::vector<unsigned char>& in, std::vector<unsigned char>& decomp){
    }
}