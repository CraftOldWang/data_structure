#include <fstream>
#include <iostream>

// 测试fstream对象的eof的行为,要点如下
// 读完所有内容后,再读,eof才会被设置为true(毕竟在读取之前,谁知道有没有达到末尾呢?)
int main()
{
    std::ifstream inputfile("txt");

    char ch;
    int count = 0;

    // 读取文件中的字符
    while (inputfile >> ch) {
        std::cout << ch << " "; // 打印读取到的字符
        count++;
    }

    std::cout << "\nTotal characters read: " << count << std::endl;

    // 并不会打印这一行,也就是说当读完所有内容后,再读,eof才会被设置(毕竟在读取之前,谁知道有没有达到末尾呢?)
    if (inputfile.eof()) {
        std::cout << "End of file reached!" << std::endl;
    }

    return 0;
}