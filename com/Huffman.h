#include "defines.h"
namespace Huff{

//typedefもっとしたい
typedef std::vector<std::pair<int, unsigned int>>  n_tree_table;
typedef std::vector<std::vector<unsigned int>> depth_table;

    class tree{
        public:
        int m_num;
        unsigned int m_val;
        int m_depth;
        //右を小さくする
        int m_parent,m_left,m_right;
        tree(){
            m_num = 0;
            m_val = 0;
            m_depth = 0;
           m_parent = -1;
           m_left = -1;
           m_right = -1;
        }
        bool operator==(tree cmp_tree){
            return this->m_num == cmp_tree.m_num && this->m_val == cmp_tree.m_val;
        }
        void add(){
            m_num++;
        }
        bool Isparent(){
            return m_parent != -1;
        }
        bool Ischild(){
            return (m_left != -1) || (m_right != -1);
        }
        int Isnum(){
            return m_num;
        }
        unsigned int Isval(){
            return m_val;
        }
        void Setparent(int parent){
            m_parent = parent;
        }
        void Setright(int right){
            m_right = right;
        }
        void Setleft(int left){
            m_left = left;
        }
        void Setnum(int num){
            m_num = num;
        }
        void Setval(unsigned int val){
            m_val = val;
        }
        int Callparent(){
            return m_parent;
        }
        bool child_check(int child){
            if(m_right != -1){
                return m_right == child;
            }else{
                if(m_left != -1){
                    return !(m_left == child);
                }else{
                    return m_right == child;
                }
            }

        }
        void print(std::vector<tree>& m_tree){
            if(Ischild()){
                std::cout << "dammy num: " << m_num << std::endl;
                if(m_right != -1){
                    std::cout << "right" << std::endl;
                    m_tree[m_right].print(m_tree);
                }
                if(m_left != -1){
                    std::cout << "left" << std::endl;
                    m_tree[m_left].print(m_tree);
                }
            }else{
                std::cout << "val: " << (int)m_val  << " num: " << m_num << " dp: " << m_depth << std::endl;
            }
        }
        int set_depth_recursive(int d, std::vector<tree>& m_tree){
            m_depth = d;
            int right_d = d;
            int left_d = d;
            if(m_right != -1){
                right_d = m_tree[m_right].set_depth_recursive(d+1,m_tree);
            }
            if(m_left != -1){
                left_d = m_tree[m_left].set_depth_recursive(d+1,m_tree);
            }
            return std::max(right_d, left_d);
        }
        void push_depth_recursive(depth_table& heap, std::vector<tree>& m_tree){
            if(m_right != -1){
                m_tree[m_right].push_depth_recursive(heap, m_tree);
            }
            if(m_left != -1){
                m_tree[m_left].push_depth_recursive(heap, m_tree);
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
        ~n_tree(){
            if(this == NULL){
                return;
            }
            delete m_right;
            delete m_left;
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

        //tableに圧縮結果を格納する
        void table_input(n_tree_table& table,int d, int root){

            if(m_right == nullptr && m_left == nullptr){
               table[m_val] = std::pair<int, unsigned int>(d, root); 
            }

            if(m_right != nullptr){
                int n_root = root;
                n_root <<= 1;
                n_root |= 1;
                m_right->table_input(table,d+1,n_root);
            }
            if(m_left != nullptr){
                int n_root = root;
                n_root <<= 1;
                m_left->table_input(table,d+1,n_root);
            }
        }

    };
    //ハフマン木から深さごとに集計する
    void normalize_tree(std::vector<tree> &hist, depth_table &comp_tree){
        int max_depth = hist[hist.size()-1].set_depth_recursive(0,hist);
        comp_tree.resize(max_depth+1);
        hist[hist.size()-1].push_depth_recursive(comp_tree,hist);
        for(int i=0;i<=max_depth;i++){
            std::sort(comp_tree[i].begin(), comp_tree[i].end());
        }
    }

    //深さごとに集計した配列から正規化したハフマン木を作る
    n_tree* make_normalized_tree(depth_table &comp_tree){
        
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
                         
            for(int j=0;j<depth_trees.size()/2;j++){                    
                n_tree* tmp = new n_tree(0,depth_trees[2*j+1],depth_trees[2*j]);
                overload_trees.push_back(tmp);
            }
        }
        return overload_trees[0];
    }

    //ヒストグラムから最小値を2つ見つける
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

    //ヒストグラムからナイーブなハフマン木を作る
    void make_huff_tree(std::vector<tree> &m_tree){
        //treeの作成
        int min_index1 = 0;
        int min_index2 = 0;
        min_search(m_tree, &min_index1, &min_index2);
        while(min_index1 != -1 && min_index2 != -1){
            min_search(m_tree, &min_index1, &min_index2);
            
            if(min_index1 != -1 && min_index2 != -1){
                tree* temp_tree = new tree();
                temp_tree->Setright(min_index1);
                temp_tree->Setleft(min_index2);
                temp_tree->Setnum(m_tree[min_index1].Isnum()+m_tree[min_index2].Isnum());
                m_tree.push_back(*temp_tree);
                m_tree[min_index1].Setparent(m_tree.size()-1);
                m_tree[min_index2].Setparent(m_tree.size()-1);
            }else{
                break;
            }
        }
    }

    //bit単位でfile入力するマン
    bool push_bit_entry(std::vector<unsigned char>& comp,unsigned char* buffer, int* line, unsigned int push_line, int push_line_size){
        int left_size = push_line_size;
        unsigned char m_buffer = *buffer;
        int m_line = *line; 
        while(left_size + m_line >= 8){
            int t_push_size = 8 - m_line;
            m_buffer <<= t_push_size;
            int t_in = push_line & ( (1 << left_size) - 1);
            t_in >>= (left_size - t_push_size);
            m_buffer |= t_in;
            comp.push_back(m_buffer);
            m_buffer = 0;
            left_size -= t_push_size;
            m_line = 0;
        }
        if(left_size > 0){
            m_buffer <<= left_size;
            int t_in = (push_line & ((1 << left_size) -1));   
            m_buffer |= t_in;
            *line += left_size;
            *buffer = m_buffer;
        }else{
            *line = m_line;
            *buffer = m_buffer;
        }
    }

    //2種類のハフマン化を施す
    void compress_to_file(std::vector<unsigned char>& file, n_tree_table &tree_top1, n_tree_table& tree_top2, std::vector<unsigned char>& comp){
        unsigned char buffer = 0;
        int line  = 0;
        //pairは長さ, 文字
        for(int i=0;i<file.size();i++){
            if((file[i] >> 7) == 1){                
                short comp_line = ((short)file[i] << 8) | (short)file[i+1];
                unsigned int head = (unsigned int)((comp_line & windowsize_mask) >> length_bit);
                unsigned int len = (unsigned int)(comp_line & length_mask);
                std::pair<int,unsigned int> comp_line_pair = tree_top2[(unsigned int)head];
                i++;
                push_bit_entry(comp,&buffer,&line,1,1);
                push_bit_entry(comp,&buffer,&line,comp_line_pair.second,comp_line_pair.first);
                push_bit_entry(comp,&buffer,&line,len,length_bit);
                
                
            }else{
                std::pair<int,unsigned int> comp_line_pair = tree_top1[(unsigned int)file[i]];
                push_bit_entry(comp,&buffer,&line,0,1);
                push_bit_entry(comp,&buffer,&line,comp_line_pair.second,comp_line_pair.first);
            }
        }

        if(line < 8){
            buffer <<= (8-line);
        }
        comp.push_back(buffer);            
    }

    //ハフマン木の情報を先頭に書き込み
    //<深さ,値>のセットを深さの小さい順に並べていく, 終端は<0,0>大きさはそれぞれのbit
    //<8bit,8bit> × M, <12bit,12bit> × Nになる
    void  write_tree_to_file(depth_table& n_tree_top, int bit_size, unsigned char *into_buffer, int *line_size, std::vector<unsigned char>& comp){
        int num_tree = 0;
        for(int i=0;i<n_tree_top.size();i++){
            num_tree += n_tree_top[i].size();
            for(int j=0;j<n_tree_top[i].size();j++){
                push_bit_entry(comp,into_buffer,line_size,(unsigned int)i,bit_size);
                push_bit_entry(comp,into_buffer,line_size,(unsigned int)n_tree_top[i][j],bit_size);
            }
        }
        push_bit_entry(comp,into_buffer,line_size,0,bit_size);
        push_bit_entry(comp,into_buffer,line_size,0,bit_size);
        //treeのsizeを出力する
        std::cout << "output tree size: " << num_tree*bit_size*2/8 << "byte" << std::endl;
    }

    //main
    void Huff_compress(std::vector<unsigned char>& file, std::vector<unsigned char>& comp){

        //集計＆正規化前のハフマン木
        std::vector<tree> tree1(1 << word_size_bit);
        std::vector<tree> tree2(1 << windowsize_bit);

        //histgram作成
        for(int i=0;i<file.size();i++){
            if((file[i] >> 7) == 1){
                short comp_line = ((short)file[i] << 8) | (short)file[i+1];
                int head = (int)((comp_line & windowsize_mask) >> length_bit);
                tree2[head].add();
                tree2[head].Setval(head);
                i++;
            }else{
                tree1[(unsigned int)file[i]].add();
                tree1[(unsigned int)file[i]].Setval((unsigned int)file[i]);
            }
        }

        make_huff_tree(tree1);
        make_huff_tree(tree2);
        //std::cout << "tree1: size " << tree1.size() <<std::endl;
        //tree1[tree1.size()-1].print(tree1);
        //std::cout << "tree2: size " << tree2.size() <<std::endl;
        //tree2[tree2.size()-1].print(tree2);  

        //深さでソートした要素集
        //圧縮したファイルのヘッダーにこれを入れる
        depth_table comp1;
        depth_table comp2;

        //treeの葉を深さごとに分類
        normalize_tree(tree1, comp1);
        normalize_tree(tree2, comp2);
        //free
        std::vector<tree>().swap(tree1);
        std::vector<tree>().swap(tree2);

        //正規化したハフマン木を作成
        n_tree* n_tree_top1 = make_normalized_tree(comp1);
        n_tree* n_tree_top2 = make_normalized_tree(comp2);
        //std::cout << "n_tree1" << std::endl;
        //n_tree_top1->print(0);
        //std::cout << "n_tree2" << std::endl;
        //n_tree_top2->print(0);
        
        //圧縮をO(1)で行うためのテーブル
        n_tree_table n_tree_table1(1 << word_size_bit);
        n_tree_table n_tree_table2(1 << windowsize_bit);

        //テーブルに圧縮情報をinput
        n_tree_top1->table_input(n_tree_table1,0,0);
        n_tree_top2->table_input(n_tree_table1,0,0);

        //free
        delete n_tree_top1;
        delete n_tree_top2;

        //treeの情報をcompに書きだす
        unsigned char m_buffer = 0;
        int m_line_size = 0;
        write_tree_to_file(comp1,word_size_bit,&m_buffer,&m_line_size,comp);
        write_tree_to_file(comp2,windowsize_bit,&m_buffer,&m_line_size,comp);
        
        //tableを使ってfileの文字をcompに書き出す
        compress_to_file(file,n_tree_table1,n_tree_table2, comp);
    
        //free
        depth_table().swap(comp1);
        depth_table().swap(comp2);
        n_tree_table().swap(n_tree_table1);
        n_tree_table().swap(n_tree_table2);
    }

    void reading_tree(std::vector<unsigned char>& in, int *index, unsigned char*buffer, int*line_size, depth_table &table){

    }

    void decompress(std::vector<unsigned char>& in, int *index, unsigned char*buffer, int*line_size, n_tree* top1, n_tree* top2, std::vector<unsigned char>& decomp){

    }    

    void Huff_decompress(std::vector<unsigned char>& in, std::vector<unsigned char>& decomp){
        
        depth_table table1;
        depth_table table2;

        unsigned char m_buffer = 0;
        int m_line_size = 0;
        int m_index = 0;
        //ヘッダーのtreeを読み取る
        reading_tree(in,&m_index,&m_buffer,&m_line_size,table1);
        reading_tree(in,&m_index,&m_buffer,&m_line_size,table2);

        //depthのtableからハフマン木を作成
        n_tree* n_tree_top1 = make_normalized_tree(table1);
        n_tree* n_tree_top2 = make_normalized_tree(table2);

        depth_table().swap(table1);
        depth_table().swap(table2);

        //ハフマン木を用いて解凍
    


        delete n_tree_top1;
        delete n_tree_top2;


    }
}