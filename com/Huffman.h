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
        unsigned int decompress(unsigned int buffer, int* line_size){
            if(m_right == nullptr && m_left == nullptr){
                //std::cout << "n_tree: buffer: " << buffer << " size: " << (*line_size) << " val: " << (int)m_val << std::endl;
                return m_val;
            }
            if(buffer < (1 << 31) ){
                buffer <<= 1;
                (*line_size) += 1;
                assert(m_left != nullptr);
                //std::cout <<" l ";
                return m_left->decompress(buffer, line_size);
            }else{
                buffer <<= 1;
                (*line_size) += 1;
                assert(m_right != nullptr);
                //std::cout <<" r ";
                return m_right->decompress(buffer, line_size);                
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
    void push_bit_entry(std::vector<unsigned char>& comp,unsigned char* buffer, int* line, unsigned int push_line, int push_line_size){
        int left_size = push_line_size;
        unsigned char m_buffer = *buffer;
        int m_line = *line; 
        while(left_size + m_line >= 8 ){
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
            *line = m_line + left_size;
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

        int sum_line_len = 0;
        //pairは長さ, 文字
        for(int i=0;i<file.size();i++){
            if((file[i] >> 7) == 1){                
                short comp_line = ((short)file[i] << 8) | (short)file[i+1];
                unsigned int head = (unsigned int)((comp_line & windowsize_mask) >> length_bit);
                unsigned int len = (unsigned int)(comp_line & length_mask);
                std::pair<int,unsigned int> comp_line_pair = tree_top2[(unsigned int)head];
                i++;
                //std::cout << "not comp line: 1" << std::endl;
                push_bit_entry(comp,&buffer,&line,1,1);
                //std::cout << "not comp line head: " << head << std::endl;
                push_bit_entry(comp,&buffer,&line,comp_line_pair.second,comp_line_pair.first);
                //std::cout << "not comp line len: " << len << std::endl;  
                push_bit_entry(comp,&buffer,&line,len,length_bit);

                sum_line_len += (1+3+comp_line_pair.first);
                
            }else{
                std::pair<int,unsigned int> comp_line_pair = tree_top1[(unsigned int)file[i]];
                //std::cout << "not comp line: 0" << std::endl;
                push_bit_entry(comp,&buffer,&line,0,1);
                //std::cout << "not comp line file: " << (unsigned int)file[i] << std::endl;
                push_bit_entry(comp,&buffer,&line,comp_line_pair.second,comp_line_pair.first);

                sum_line_len += (comp_line_pair.first);
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
    int write_tree_to_file(depth_table& n_tree_top, int bit_size, unsigned char *into_buffer, int *line_size, std::vector<unsigned char>& comp){
        int num_tree = 0;
        for(int i=n_tree_top.size()-1;i>=0;i--){
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
        return num_tree*bit_size*2/8;
    }

    //main
    void Huff_compress(std::vector<unsigned char>& file, std::vector<unsigned char>& comp){

        //先頭に非圧縮でのファイルサイズを入力(16bit)
        unsigned int init_size = file.size();
        unsigned char s1 = (unsigned char)(init_size >> 8);
        unsigned char s2 = (unsigned char)(init_size & 0xff);
        comp.push_back(s1);
        comp.push_back(s2);
        std::cout << "huff filesize: " << init_size << std::endl;
        std::cout << "s1: " << (unsigned int)s1 << " s2: " << (unsigned int)s2 << std::endl;

        //集計＆正規化前のハフマン木
        std::vector<tree> tree1(1 << word_size_bit);
        std::vector<tree> tree2(1 << windowsize_bit);

        //histgram作成
        for(int i=0;i<file.size();i++){
            if(((file[i] >> 7) & 1)  == 1){
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

        //深さでソートした要素集
        //圧縮したファイルのヘッダーにこれを入れる
        depth_table comp1_table;
        depth_table comp2_table;

        //treeの葉を深さごとに分類
        normalize_tree(tree1, comp1_table);
        normalize_tree(tree2, comp2_table);
        //free
        std::vector<tree>().swap(tree1);
        std::vector<tree>().swap(tree2);

        //正規化したハフマン木を作成
        n_tree* n_tree_top1 = make_normalized_tree(comp1_table);
        n_tree* n_tree_top2 = make_normalized_tree(comp2_table);

        //圧縮をO(1)で行うためのテーブル
        n_tree_table n_tree_table1(1 << word_size_bit);
        n_tree_table n_tree_table2(1 << windowsize_bit);

        //テーブルに圧縮情報をinput
        n_tree_top1->table_input(n_tree_table1,0,0);
        n_tree_top2->table_input(n_tree_table2,0,0);

        //free
        delete n_tree_top1;
        delete n_tree_top2;

        //treeの情報をcompに書きだす
        unsigned char m_buffer = 0;
        int m_line_size = 0;
        int tree_size_sum = 0;
        tree_size_sum += write_tree_to_file(comp1_table,word_size_bit,&m_buffer,&m_line_size,comp);
        tree_size_sum += write_tree_to_file(comp2_table,windowsize_bit,&m_buffer,&m_line_size,comp);
        
        //tableを使ってfileの文字をcompに書き出す
        compress_to_file(file,n_tree_table1,n_tree_table2, comp);
    
        std::cout << "not tree huffman size: " << comp.size() - tree_size_sum << "byte" << std::endl;
        //free
        depth_table().swap(comp1_table);
        depth_table().swap(comp2_table);
        n_tree_table().swap(n_tree_table1);
        n_tree_table().swap(n_tree_table2);
    }

    void reading_tree(std::vector<unsigned char>& in, int *index, unsigned int*buffer, int*line_size, depth_table &table, int bit_size){
        
        int i=(*index);
        bool is_first = true;
        
        while(i - (*index) < (1 << bit_size)){
            unsigned short s1 = 0;
            unsigned short s2 = 0;
            if(bit_size == 8){
                s1 = in[i];
                s2 = in[i+1];
                i += 2;
            }else if(bit_size == 12) {
                unsigned char tmp1 = in[i];
                unsigned char tmp2 = in[i+1];
                unsigned char tmp3 = in[i+2];
                s1 = ((unsigned short)tmp1 << 8) | (((unsigned short)tmp2 & 0xf0 )>> 4);
                s2 = (((unsigned short)tmp2 & 0xf) << 8) | (unsigned short)tmp3;
                i += 3;
            }
            if(s1 == 0 && s2 == 0){
                break;
            }
            if(is_first){
                table.resize(s1+1);
                is_first = false;
            }
            table[s1].push_back(s2);
        }
        *index = (i);
    }

    int buffer_read(std::vector<unsigned char>& in, int *index,unsigned int*buffer, int line_size, int*buffer_flow_size){
        unsigned int m_buffer = (*buffer);
        //std::cout << "buffer_read start" << std::endl;
        //std::cout << "m_buffer: " << (*buffer) << std::endl;
        //std::cout << "buffer_flow_size: " << (*buffer_flow_size) << std::endl;
        //std::cout << "line_size: " << line_size << std::endl;
        while((*buffer_flow_size) + line_size >= 8 && in.size() > (*index)){
            unsigned char temp = in[(*index)];
            int m_flow = 8 -(*buffer_flow_size);
            temp &= (1<<( m_flow )-1);
            temp <<= (line_size - m_flow);

            //std::cout <<"w_temp: " << (unsigned int)temp << std::endl;
            m_buffer |= (unsigned int)temp;

            line_size -= m_flow;
            (*buffer_flow_size) = 0;
            (*index) = (*index) + 1 ;
        }
        if(line_size > 0){
            unsigned char temp = in[(*index)];
            int m_flow = 8 -(*buffer_flow_size);
            temp &= ((1<< m_flow )-1);
            temp >>= (m_flow - line_size);
            //std::cout <<"i_temp: " << (unsigned int)temp << std::endl;
            m_buffer |=  (unsigned int)temp;
            (*buffer_flow_size) += line_size;
        }
        (*buffer) = m_buffer;
        //std::cout << "m_buffer: " << (*buffer) << std::endl;
        //std::cout << "buffer_read end" << std::endl;    
        if(in.size() >= (*index) && (*buffer_flow_size) >= 8){
             return 32 - line_size;
        }else{
            return 32;
        }
    }

    void decompress(std::vector<unsigned char>& in, int *index, unsigned int*buffer, int*line_size, n_tree* top1, n_tree* top2, std::vector<unsigned char>& decomp, int huff_size){
        int m_index = (*index);
        unsigned int m_buffer = (*buffer);
        int buffer_flow_size = 0;
        int left_buffer_size = 32;
        m_buffer = 0;
        //init
        m_buffer |= (unsigned int)in[m_index++];
        m_buffer <<= 8;
        m_buffer |= (unsigned int)in[m_index++];
        m_buffer <<= 8;
        m_buffer |= (unsigned int)in[m_index++];
        m_buffer <<= 8;
        m_buffer |= (unsigned int)in[m_index++];        

        int it = 0;
        while(left_buffer_size > 0 && it < huff_size){
            //std::cout << "ini_buffer " << m_buffer << std::endl;
            int using_buffer = 0;
            //LZSS
            if(m_buffer >> 31 == 1){
                m_buffer <<= 1;
                int m_line_size = 0;
                unsigned int val = top2->decompress(m_buffer, &m_line_size);
                m_buffer <<= m_line_size;
                unsigned short lzss = 1;
                lzss <<= windowsize_bit;
                lzss |= (unsigned short)val;
                lzss <<= length_bit;
                lzss |= (unsigned short)(m_buffer >> (32-length_bit));
                
                unsigned char s1 = lzss >> 8;
                unsigned char s2 = lzss & 0xff;
                decomp.push_back(s1);
                decomp.push_back(s2);
                //std::cout << "s1: " << (int)s1 << " s2: " << (int)s2 << std::endl;
                m_buffer <<= length_bit;
                using_buffer = 1 + m_line_size + length_bit;
                if(left_buffer_size == 32){
                    left_buffer_size = buffer_read(in, &m_index, &m_buffer, using_buffer, &buffer_flow_size);
                }
                it += 2;
            }else{
                m_buffer <<= 1;
                int m_line_size = 0;
                unsigned int val = top1->decompress(m_buffer, &m_line_size);
                decomp.push_back((unsigned char)val);
                //std::cout << "val: " << (int)val << std::endl;
                m_buffer <<= m_line_size;
                using_buffer = 1 + m_line_size;
                if(left_buffer_size == 32){
                    left_buffer_size = buffer_read(in, &m_index, &m_buffer, using_buffer, &buffer_flow_size);
                }
                it++;
            }
            if(in.size() >= m_index && buffer_flow_size >= 8){
                left_buffer_size -= using_buffer;
            }
        }
        (*index) = m_index;
        (*buffer) = m_buffer;
    }    

    void Huff_decompress(std::vector<unsigned char>& in, std::vector<unsigned char>& decomp){
        
        depth_table table1;
        depth_table table2;

        unsigned int m_buffer = 0;
        int m_line_size = 0;
        int m_index = 0;

        //ファイルサイズを取得する
        int huff_size;
        unsigned char s1;
        unsigned char s2;
        s1 = in[m_index++];
        s2 = in[m_index++];
        huff_size = ((unsigned int)s1 << 8) | (unsigned int)s2;
        std::cout << "huff size: " << huff_size << std::endl;

        //ヘッダーのtreeを読み取る
        reading_tree(in,&m_index,&m_buffer,&m_line_size,table1,word_size_bit);
        reading_tree(in,&m_index,&m_buffer,&m_line_size,table2,windowsize_bit);
        //depthのtableからハフマン木を作成
        n_tree* n_tree_top1 = make_normalized_tree(table1);
        n_tree* n_tree_top2 = make_normalized_tree(table2);
        
        //bufferがint(32bit)じゃ足りなくなる
        assert(table1.size() <= 32);
        assert(table2.size() <= 32);
        depth_table().swap(table1);
        depth_table().swap(table2);

        //ハフマン木を用いて解凍
        decompress(in,&m_index,&m_buffer,&m_line_size,n_tree_top1,n_tree_top2,decomp, huff_size);

        delete n_tree_top1;
        delete n_tree_top2;
    }
}