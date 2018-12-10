#include<vector>
#include<iostream>
#include<math.h>
#include<algorithm>
namespace Huff{

    class tree{
        public:
        int m_num;
        unsigned char m_val;
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
        void Setval(unsigned char val){
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
        void push_depth_recursive(std::vector<std::vector<unsigned char>>& heap){
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

    void normalize_tree(std::vector<tree> &hist, std::vector<std::vector<unsigned char>> &comp_tree){
        int max_depth = hist[hist.size()-1].set_depth_recursive(0);
        comp_tree.resize(max_depth+1);
        hist[hist.size()-1].push_depth_recursive(comp_tree);
        for(int i=0;i<=max_depth;i++){
            std::sort(comp_tree[i].begin(), comp_tree[i].end());
        }/*
        for(int i=0;i<comp_tree.size();i++){
            std::cout << "dp: " << i << std::endl;
            for(auto it = comp_tree[i].begin();it != comp_tree[i].end();it++){
                std::cout << "val: " << (int)(*it) << std::endl;
            }
        }
        */
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

    void Huff_compress(std::vector<unsigned char>& file, std::vector<unsigned char>& comp){

        std::vector<tree> m_tree(256,tree());
        //histgram作成
        for(int i=0;i<file.size();i++){
            m_tree[(int)file[i]].add();
            m_tree[(int)file[i]].Setval(file[i]);
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

        int line= 0;
        int bits = 0;
        /*
        for(auto it = m_tree.begin();it != m_tree.end();it++){
            if(!it->Ischild() && it->Isnum() != 0){
                compress(m_tree,&line,&bits,it->m_val);
                std::cout << "value: " << (it->m_val) << " line: " << line << " size: " << bits <<std::endl;
            }
        }
        */
        std::vector<std::vector<unsigned char>> comp1;
        normalize_tree(m_tree, comp1);

        //treeの情報をcompに書きだす

        //treeを使ってfileの文字をcompに書き出す


        m_tree[m_tree.size()-1].print();
        std::vector<tree>().swap(m_tree);
        std::vector<std::vector<unsigned char>>().swap(comp1);
    }
    void Huff_decompress(std::vector<unsigned char>& in, std::vector<unsigned char>& decomp){


    }
}