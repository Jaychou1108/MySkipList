#include "SkipList.h"

int main() {
    // 键值中的key用int型，如果用其他类型，需要自定义比较函数
    // 而且如果修改key的类型，同时需要修改skipList.load_file函数
    SkipList<int, std::string> skipList(6);
	skipList.insert_element(1, "小"); 
	skipList.insert_element(3, "头头"); 
	skipList.insert_element(7, "是"); 
	skipList.insert_element(8, "一只"); 
	skipList.insert_element(9, "活泼可爱"); 
	skipList.insert_element(19, "但是"); 
	skipList.insert_element(20, "喜欢咬人的小猫咪"); 
    

    std::cout << "skipList size:" << skipList.size() << std::endl;

    skipList.dump_file();
    // skipList.load_file();

    skipList.search_element(9);
    skipList.search_element(18);


    skipList.display_list();

    skipList.delete_element(3);
    skipList.delete_element(7);

    std::cout << "skipList size:" << skipList.size() << std::endl;

    skipList.display_list();
    
}