#include "defines.h"
namespace Huff{

    class tree{
        public:
        int m_num;
        unsigned int m_val;
        int m_depth;
        //右を小さくする
        tree* m_parent;
        tree* m_left;
        tree* m_right;
        tree(){
            m_num = 0;
            m_val = 0;
            m_depth = 0;
            m_parent = nullptr;
            m_left = nullptr;
            m_right = nullptr;
        }
        bool operator==(tree cmp_tree){
            return this->m_num == cmp_tree.m_num && this->m_val == cmp_tree.m_val;
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
        unsigned int Isval(){
            return m_val;
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
        void Setval(unsigned int val){
            m_val = val;
        }
        tree* Callparent(){
            return m_parent;
        }
        bool child_check(tree child){
            if(m_right != nullptr){
                return *m_right == child;
            }else{
                if(m_left != nullptr){
                    return !(*m_left == child);
                }else{
                    return *m_right == child;
                }
            }

        }
        void print(){
            if(Ischild()){
                std::cout << "dammy num: " << m_num << std::endl;
                if(m_right != nullptr){
                    std::cout << "right" << std::endl;
                    m_right->print();
                }
                if(m_left != nullptr){
                    std::cout << "left" << std::endl;
                    m_left->print();
                }
            }else{
                std::cout << "val: " << (int)m_val  << " num: " << m_num << " dp: " << m_depth << std::endl;
            }
        }
        int set_depth_recursive(int d){
            m_depth = d;
            int right_d = d;
            int left_d = d;
            if(m_right != nullptr){
                right_d = m_right->set_depth_recursive(d+1);
            }
            if(m_left != nullptr){
                left_d = m_left->set_depth_recursive(d+1);
            }
            return std::max(right_d, left_d);
        }
        void push_depth_recursive(std::vector<std::vector<unsigned int>>& heap){
            if(m_right != nullptr){
                m_right->push_depth_recursive(heap);
            }
            if(m_left != nullptr){
                m_left->push_depth_recursive(heap);
            }
            if(!Ischild()){
                heap[m_depth].push_back(m_val);
            }
        }
    };

    class n_tree{
        public:
        n_tree* m_right;
        n_tree* m_left;
        unsigned int m_val;
        n_tree(unsigned int val=0, n_tree* right=nullptr, n_tree*left=nullptr){
            m_val = val;
            m_right = right;
            m_left = left;
        }
        void print(int i){
            if(m_right != nullptr){
                std::cout << "right " << std::endl;
                m_right->print(i+1);
            }
            if(m_left != nullptr){
                std::cout << "left " << std::endl;
                m_left->print(i+1);
            }
            if(m_left == nullptr && m_right == nullptr){
                std::cout << "val: " << m_val << std::endl;
            }
        }
        //(d,root)
        std::pair<int,int> search(unsigned int target, int d=0, int root=0){
            std::pair<int,int> r,l;
            r = std::pair<int,int>(0,0);
            l = std::pair<int,int>(0,0);
            if(m_right != nullptr){
                root |= (1 << d+1);
                r = m_right->search(target,d+1,root);
            }   
            if(m_left != nullptr){
                l = m_left->search(target,d+1,root);
            }
            if(m_left == nullptr && m_right == nullptr){
                if(target == m_val){
                    return std::pair<int,int>(d,root);
                }else{
                    return std::pair<int,int>(0,0);
                }
            }
            if(r.first != 0){
                return r;
            }else{
                return l;
            }
        }
    };

    void normalize_tree(std::vector<tree> &hist, std::vector<std::vector<unsigned int>> &comp_tree){
        int max_depth = hist[hist.size()-1].set_depth_recursive(0);
        comp_tree.resize(max_depth+1);
        hist[hist.size()-1].push_depth_recursive(comp_tree);
        for(int i=0;i<=max_depth;i++){
            std::sort(comp_tree[i].begin(), comp_tree[i].end());
        }
        /*
        for(int i=0;i<comp_tree.size();i++){
            std::cout << "dp: " << i << std::endl;
            for(auto it = comp_tree[i].begin();it != comp_tree[i].end();it++){
                std::cout << "val: " << (int)(*it) << std::endl;
            }
        }
        */
    }

    n_tree* make_normalized_tree(std::vector<std::vector<unsigned int>> &comp_tree){
        
        int d_size = comp_tree.size();
        std::vector<n_tree*> overload_trees;
        for(int i=d_size-1; i>0;i--){

            std::vector<n_tree*> depth_trees;
            for(auto it = comp_tree[i].begin();it != comp_tree[i].end();it++){
                n_tree* tmp = new n_tree(*it,nullptr,nullptr);
                depth_trees.push_back(tmp);
            }
            std::copy(overload_trees.begin(),overload_trees.end(), std::back_inserter(depth_trees));
            std::vector<n_tree*>().swap(overload_trees);
            overload_trees.resize(0);
            std::cout << "depth: " << i << " d_tree_size: " << depth_trees.size() << std::endl;             
            for(int j=0;j<depth_trees.size()/2;j++){                    
                n_tree* tmp = new n_tree(0,depth_trees[2*j+1],depth_trees[2*j]);
                overload_trees.push_back(tmp);
            }
            std::cout << "depth: " << i << " o_tree_size: " << overload_trees.size() << std::endl;             
        }
        //overload_trees[0]->print(0);
        return overload_trees[0];
    }

    void compress(std::vector<tree> &hist, int*comp_line, int*comp_bit, unsigned char target){

        
        int m_comp_line = 0;
        int m_comp_bit = 0;
        tree *init_tree = &hist[(int)target];
        while(init_tree->Isparent()){
            if(init_tree->Isparent()){
                tree *parent = init_tree->Callparent();
                if(parent->child_check(*init_tree)){
                    m_comp_line |= (1 << m_comp_bit);
                }
                m_comp_bit++;
                init_tree = parent;
            }else{
                break;
            }
        }
        *comp_bit = m_comp_bit;
        *comp_line = m_comp_line;
        
    }

    void min_search(std::vector<tree> &hist, int*min_index1, int*min_index2){
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

    void make_huff_tree(std::vector<tree> &m_tree){
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

    bool push_bit_entry(std::vector<unsigned char>& comp,unsigned char* buffer, int* line, unsigned int push_line, int push_line_size){
                
    }

    //2種類のハフマン化を施す
    void compress_to_file(std::vector<unsigned char>& file, n_tree* tree_top1, n_tree* tree_top2, std::vector<unsigned char>& comp){
        //32bit
        unsigned char buffer = 0;
        int line  = 0;
        //pairは長さ, 文字
        for(int i=0;i<file.size();i++){
            if((file[i] >> 7) == 1){                
                short comp_line = ((short)file[i] << 8) | (short)file[i+1];
                unsigned int head = (unsigned int)((comp_line & windowsize_mask) >> length_bit);
                unsigned int len = (unsigned int)(comp_line & length_mask);
                std::pair<int,int> comp_line_pair = tree_top2->search((unsigned int)head);
                i++;

                if(line < 8){
                    buffer <<= 1;
                    buffer |= 1;
                    line++;
                }else{
                    comp.push_back(buffer);
                    buffer = 0;
                    line = 0;
                }
                for(int i=comp_line_pair.first;i>0;i--){
                    if(line < 8){
                        buffer <<= 1;
                        buffer |= ((comp_line_pair.second >> (i-1)) & 1);
                    }else{
                        comp.push_back(buffer);
                        buffer = 0;
                        line = 0;
                    }
                }
                for(int i=length_bit;i>0;i--){
                    if(line < 8){
                        buffer <<= 1;
                        buffer |= ((len >> (i-1)) & 1);
                    }else{
                        comp.push_back(buffer);
                        buffer = 0;
                        line = 0;
                    }
                }
            }else{
                std::pair<int,int> comp_line_pair = tree_top1->search((unsigned int)file[i]);
                if(line < 8){
                    buffer <<= 1;
                    //buffer |= 1;
                    line++;
                }else{
                    comp.push_back(buffer);
                    buffer = 0;
                    line = 0;
                }
                for(int i=comp_line_pair.first;i>0;i--){
                    if(line < 8){
                        buffer <<= 1;
                        buffer |= ((comp_line_pair.second >> (i-1)) & 1);
                    }else{
                        comp.push_back(buffer);
                        buffer = 0;
                        line = 0;
                    }
                }

            }
        }   
        if(line < 8){
            buffer <<= (8-line);
            comp.push_back(buffer);
        }    
    }


    void Huff_compress(std::vector<unsigned char>& file, std::vector<unsigned char>& comp){

        std::vector<tree> tree1((1 << word_size_bit),tree());
        std::vector<tree> tree2((1 << windowsize_bit),tree());

        //histgram作成
        for(int i=0;i<file.size();i++){
            if((file[i] >> 7) == 1){
                short comp_line = ((short)file[i] << 8) | (short)file[i+1];
                unsigned int head = (unsigned int)((comp_line & windowsize_mask) >> length_bit);
                tree2[head].add();
                tree2[head].Setval(head);
                i++;
            }else{
                tree1[(unsigned int)file[i]].add();
                tree1[(unsigned int)file[i]].Setval(file[i]);
            }
        }
        make_huff_tree(tree1);
        make_huff_tree(tree2);
        std::vector<std::vector<unsigned int>> comp1;
        std::vector<std::vector<unsigned int>> comp2;
        //treeの葉を深さごとに分類
        normalize_tree(tree1, comp1);
        normalize_tree(tree2, comp2);
        //正規化したハフマン木を作成
        n_tree* n_tree_top1 = make_normalized_tree(comp1);
        n_tree* n_tree_top2 = make_normalized_tree(comp2);

        /*
        n_tree_top->print(0); 
        std::pair<int,int> a = n_tree_top->search(97);
        std::pair<int,int> b = n_tree_top->search(98);
        std::pair<int,int> c = n_tree_top->search(99);
        std::cout << "a: " << a.first << " " << a.second << std::endl;        
        std::cout << "b: " << b.first << " " << b.second << std::endl;
        std::cout << "c: " << c.first << " " << c.second << std::endl;
        int line= 0;
        int bits = 0;
        */
        
        //treeの情報をcompに書きだす

        //treeを使ってfileの文字をcompに書き出す

        //tree1[tree1.size()-1].print();
        std::vector<tree>().swap(tree1);
        std::vector<tree>().swap(tree2);
        std::vector<std::vector<unsigned int>>().swap(comp1);
        std::vector<std::vector<unsigned int>>().swap(comp2);
    }
    void Huff_decompress(std::vector<unsigned char>& in, std::vector<unsigned char>& decomp){

    }
}