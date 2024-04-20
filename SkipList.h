/*
    FILE-NAME : SkipList
    author : toutou
    create_time : 2024-4-19-0:42:58
    cover : https://github.com/youngyangyang04
*/

#include <iostream> 
#include <cstdlib>
#include <cmath>
#include <cstring>
#include <mutex>
#include <fstream>

#define STORE_FILE "../store/dumpFile"

std::string delimiter = ":";
std::mutex mtx;     //互斥锁确保多线程下的数据安全

template<typename K, typename V>
class Node{
public:
    Node(){}    //默认构造函数
    Node(K k, V v, int level);  //构造函数
    K get_key() const;  //获取key
    V get_value() const;    //获取value
    void set_value(int v);  //设置value
    Node<K, V>** forward;   //层级节点
    int node_level;         //最高层级
private:
    K key;
    V value;
};

template<typename K, typename V>
Node<K, V>::Node(K k, V v, int level){
    this->key = k;
    this->value = v;
    this->node_level = level;
    this->forward = new Node<K, V>* [level + 1];
    memset(this->forward, 0, sizeof(Node<K, V>*) * (level + 1));
}

template<typename K, typename V>
K Node<K, V>::get_key() const{
    return this->key;
}

template<typename K, typename V>
V Node<K, V>::get_value() const{
    return this->value;
}

template<typename K, typename V>
void Node<K, V>::set_value(int v) {
    this->value = v;
}

template<typename K, typename V>
class SkipList {
public:
    SkipList(int max_level);  //构造函数
    int get_random_level();   //生成随机层级
    Node<K, V>* create_node(K k, V v, int level);    //创建新节点
    int insert_element(K key, V value);   //插入元素
    bool search_element(K key);     //搜索元素
    void delete_element(K key);     //删除元素
    void display_list();
    void dump_file();               //数据的持久化
    void load_file();               //数据读取
    void clear(Node<K,V>*);         //递归删除节点
    int size();                     //返回节点数量
    ~SkipList();

private:
    bool is_valid_string(const std::string& str);   //验证字符串合法性
    void get_key_value_from_string(const std::string& str, std::string* key, std::string* value);   //分割键值
    

private:
    int _max_level; //跳表的最高层数(索引，从0开始)
    int _skip_list_level;   //当前的层数(索引，从0开始)
    Node<K, V>* _header;    //头节点
    int _element_count;     //节点数量
    std::ofstream _file_writer; 
    std::ifstream _file_reader;
};

template<typename K, typename V>
SkipList<K, V>::SkipList(int max_level) {
    this->_max_level = max_level;   //设置最大层级
    this->_skip_list_level = 0;     //设置当前层级
    this->_element_count = 0;       //当前跳表节点数量
    K k;                            //默认键值
    V v;                            //默认值
    _header = new Node<K, V>(k, v, _max_level);           //头节点
}

template<typename K, typename V>
int SkipList<K, V>::get_random_level() {
    int level = 1;
    while (rand() % 2 == 1) {
        level++;
    }

    level = level < _max_level ? level : _max_level;
    return level;
}
template<typename K, typename V>
Node<K, V>* SkipList<K, V>::create_node(K k, V v, int level) {
    Node<K, V> *n = new Node<K, V>(k, v, level);
    return n;
}

template<typename K, typename V>
bool SkipList<K, V>::search_element(K key) {
    Node<K, V>* current = _header;

    for (int i = _skip_list_level; i >= 0; i--) {
        while (current->forward[i] != nullptr && current->forward[i]->get_key() < key) {
            current = current->forward[i];
        }
    }
    current = current->forward[0];
    if (current && current->get_key() == key)
        return true;
    return false;
}

template<typename K, typename V>
int SkipList<K, V>::insert_element(K key, V value) {
    std::lock_guard lock(mtx);
    Node<K, V>* current = this->_header;

    Node<K, V>* update[_max_level + 1];
    memset(update, 0, sizeof(Node<K, V>*) * (_max_level + 1));

    for (int i = _skip_list_level; i >= 0; i--) {
        while (current->forward[i] != nullptr && current->forward[i]->get_key() < key) {
            current = current->forward[i];
        }
        update[i] = current;
    }

    current = current->forward[0];
    if (current != nullptr && current->get_key() == key)
        return 1;
    
    if (current == nullptr || current->get_key() != key) {
        int random_level = get_random_level();
        if (random_level > _skip_list_level) {
            for (int i = random_level; i > _skip_list_level; i--) {
                update[i] = _header;
            }
            _skip_list_level = random_level;
        }

    Node<K, V>* insert_node = create_node(key, value, random_level);
    memset(insert_node->forward, 0, sizeof(Node<K, V>*) * (random_level + 1));

    for (int i = random_level; i >= 0; i--) {
        insert_node->forward[i] = update[i]->forward[i];
        update[i]->forward[i] = insert_node;
    }
    _element_count++;
    }
    return 0;
}

template<typename K, typename V>
void SkipList<K, V>::delete_element(K key) {
    std::lock_guard lock(mtx);
    Node<K, V>* current = _header;
    Node<K, V>* update[_max_level + 1];
    memset(update, 0, sizeof(Node<K, V>*) * (_max_level + 1));

    for (int i = _skip_list_level; i >= 0; i--) {
        while (current->forward[i] != nullptr && current->forward[i]->get_key() < key) {
            current = current->forward[i];
        }
        update[i] = current;
    }

    current = current->forward[0];
    if (current != nullptr && current->get_key() == key) {
        for (int i = 0; i <= _skip_list_level; i++) {
            if (current != update[i]->forward[i])
                break;
            update[i]->forward[i] = current->forward[i];
        }

        while (_skip_list_level > 0 && _header->forward[_skip_list_level] == 0) {
            _skip_list_level--;
        }

        std::cout << "Successfully deleted key "<< key << std::endl;
        delete current;
        _element_count--;
    }
    return;
}

template<typename K, typename V>
void SkipList<K, V>::display_list() {
    for (int i = _skip_list_level; i >= 0; i--) {
        Node<K, V>* node = _header->forward[i];
        std::cout << "Level" << i << ":";
        while (node != nullptr) {
            std::cout << node->get_key() << ":" << node->get_value() << ";";
            node = node->forward[i];
        }
        std::cout << std::endl;
    }
}

template<typename K, typename V>
void SkipList<K, V>::dump_file() {
    std::cout << "dump_file-----------------" << std::endl;
    _file_writer.open(STORE_FILE);
    if (_file_writer.is_open())
        std::cout << "open successful---------------" << std::endl;
    Node<K, V>* node = this->_header->forward[0];
    while (node != nullptr) {
        _file_writer << node->get_key() << ":" << node->get_value() << ";\n";
        node = node->forward[0];
    }
    _file_writer.flush();
    _file_writer.close();
    return;
} 

template<typename K, typename V>
bool SkipList<K, V>::is_valid_string(const std::string& str) {
    return !str.empty() && str.find(delimiter) != std::string::npos;
}

template<typename K, typename V>
void SkipList<K, V>::get_key_value_from_string(const std::string& str, std::string* key, std::string* value) {
    if (!is_valid_string(str))
        return;
    *key = str.substr(0, str.find(delimiter));
    *value = str.substr(str.find(delimiter) + 1);
}

template<typename K, typename V>
void SkipList<K, V>::load_file() {
    _file_reader.open(STORE_FILE);
    std::string line;
    std::string* key = new std::string();
    std::string* value = new std::string();

    while (getline(_file_reader, line)) {
        get_key_value_from_string(line, key, value);
        if (key->empty() || value->empty())
            continue;

        insert_element(stoi(*key), *value);
        std::cout << "key:" << *key <<  "value:" << *value << std::endl;
    }
    delete key;
    delete value;
    _file_reader.close();
}

template<typename K, typename V>
int SkipList<K, V>::size() {
    return _element_count;
}

template<typename K, typename V>
void SkipList<K, V>::clear(Node<K,V>* node) {
    if (node->forward[0] != nullptr) {
        clear(node->forward[0]);
    }
    delete node;
}

template<typename K, typename V>
SkipList<K, V>::~SkipList() {
    if (_file_reader.is_open())
        _file_reader.close();
    if (_file_writer.is_open())
        _file_writer.close();

    if (_header->forward[0] != nullptr) {
        clear(_header->forward[0]);
    }
    delete _header;
}