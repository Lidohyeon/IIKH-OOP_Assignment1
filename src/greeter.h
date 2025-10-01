#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string>
#include <vector>

class Greeter
{
private:
    std::vector<std::string> data; // 데이터를 저장할 벡터

public:
    // 생성자
    Greeter()
    {
        showMenu();
    }

    // 메뉴 표시 함수
    void showMenu()
    {
        int choice;

        while (true)
        {
            std::cout << "\n===== 메뉴 선택 =====" << std::endl;
            std::cout << "1. Search" << std::endl;
            std::cout << "2. Insert" << std::endl;
            std::cout << "3. Delete" << std::endl;
            std::cout << "4. Exit" << std::endl;
            std::cout << "선택하세요: ";

            std::cin >> choice;

            switch (choice)
            {
            case 1:
                searchData();
                break;
            case 2:
                insertData();
                break;
            case 3:
                deleteData();
                break;
            case 4:
                std::cout << "프로그램을 종료합니다." << std::endl;
                return;
            default:
                std::cout << "잘못된 선택입니다. 다시 선택해주세요." << std::endl;
                break;
            }
        }
    }

    // 데이터 검색 함수
    void searchData()
    {
        if (data.empty())
        {
            std::cout << "저장된 데이터가 없습니다." << std::endl;
            return;
        }

        std::string searchTerm;
        std::cout << "검색할 데이터를 입력하세요: ";
        std::cin.ignore(); // 이전 입력의 개행문자 제거
        std::getline(std::cin, searchTerm);

        bool found = false;
        for (size_t i = 0; i < data.size(); i++)
        {
            if (data[i].find(searchTerm) != std::string::npos)
            {
                std::cout << "찾은 데이터 [" << i << "]: " << data[i] << std::endl;
                found = true;
            }
        }

        if (!found)
        {
            std::cout << "'" << searchTerm << "'를 포함하는 데이터를 찾을 수 없습니다." << std::endl;
        }
    }

    // 데이터 삽입 함수
    void insertData()
    {
        std::string newData;
        std::cout << "추가할 데이터를 입력하세요: ";
        std::cin.ignore(); // 이전 입력의 개행문자 제거
        std::getline(std::cin, newData);

        data.push_back(newData);
        std::cout << "데이터가 성공적으로 추가되었습니다." << std::endl;
        std::cout << "현재 저장된 데이터 개수: " << data.size() << std::endl;
    }

    // 데이터 삭제 함수
    void deleteData()
    {
        if (data.empty())
        {
            std::cout << "삭제할 데이터가 없습니다." << std::endl;
            return;
        }

        // 현재 저장된 데이터 목록 표시
        std::cout << "현재 저장된 데이터:" << std::endl;
        for (size_t i = 0; i < data.size(); i++)
        {
            std::cout << "[" << i << "] " << data[i] << std::endl;
        }

        int index;
        std::cout << "삭제할 데이터의 인덱스를 입력하세요: ";
        std::cin >> index;

        if (index >= 0 && index < static_cast<int>(data.size()))
        {
            std::cout << "'" << data[index] << "'가 삭제되었습니다." << std::endl;
            data.erase(data.begin() + index);
            std::cout << "현재 저장된 데이터 개수: " << data.size() << std::endl;
        }
        else
        {
            std::cout << "유효하지 않은 인덱스입니다." << std::endl;
        }
    }

    // 소멸자
    ~Greeter()
    {
        std::cout << "Greeter 객체가 소멸됩니다." << std::endl;
    }
};