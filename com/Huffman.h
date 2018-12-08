#include<vector>


namespace Huff{
    class tree{
        public:
        int m_num;
        unsigned char m_val;
        //右を小さくする
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
        bool Ischild(){
            return (m_left != nullptr) || (m_right != nullptr);
        }
        int Isnum(){
            return m_num;
        }
        void Setparent(tree* parent){
            m_parent = parent;
        }
        void Setright(tree* right){
            m_right = right;
        }
        void Setleft(tree* left){
            m_left = left;
        }
        void Setnum(int num){
            m_num = num;
        }
    };

    void min_search(std::vector<tree> hist, int*min_index1, int*min_index2){
        int min1 = __INT_MAX__;
        int min2 = __INT_MAX__;
        int index1 = -1;
        int index2 = -1;
        for(int i=0;i<hist.size();i++){
            if(!hist[i].Isparent() && hist[i].Isnum() > 0){
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
        std::vector<tree> m_tree(256,tree());
        //histgram作成
        for(int i=0;i<file.size();i++){
            m_tree[(int)file[i]].add();
        }
        //treeの作成
        int min_index1 = -2;
        int min_index2 = -2;
        while(min_index1 != -1 && min_index2 != -1){
            min_search(m_tree, &min_index1, &min_index2);
            if(min_index1 != -1 && min_index2 != -1){
                tree temp_tree = tree();
                temp_tree.Setright(&m_tree[min_index1]);
                temp_tree.Setleft(&m_tree[min_index2]);
                temp_tree.Setnum(m_tree[min_index1].Isnum()+m_tree[min_index2].Isnum());
                m_tree.push_back(temp_tree);
                m_tree[min_index1].Setparent(&m_tree[m_tree.size()-1]);
                m_tree[min_index2].Setparent(&m_tree[m_tree.size()-1]);
            }else{
                break;
            }
        }

    }
    void Huff_decompress(std::vector<unsigned char>& in, std::vector<unsigned char>& decomp){
    }
}