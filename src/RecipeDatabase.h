
#ifndef RECIPE_DATABASE_H
#define RECIPE_DATABASE_H

#include <vector>      // std::vector: 동적으로 크기가 조절되는 배열을 사용하기 위해 포함합니다. 레시피 목록을 저장하는 데 사용됩니다.
#include <string>      // std::string: 문자열을 다루는 클래스를 사용하기 위해 포함합니다. 레시피의 제목, 재료 이름 등을 저장합니다.
#include <fstream>     // std::ifstream, std::ofstream: 파일 입출력(읽기/쓰기)을 위해 포함합니다. 레시피를 파일에 저장하고 불러오는 데 사용됩니다.
#include <sstream>     // std::istringstream: 문자열을 스트림처럼 다루기 위해 포함합니다. 문자열을 특정 구분자로 분리(파싱)할 때 유용합니다.
#include <algorithm>   // std::transform, std::sort, std::remove_if 등 범용 알고리즘 함수를 사용하기 위해 포함합니다.
#include <limits>      // std::numeric_limits: 특정 타입의 최대값 등 숫자 한계에 대한 정보를 얻기 위해 포함합니다. 입력 버퍼를 비울 때 사용됩니다.
#include "Recipe.h"    // Recipe 클래스 헤더 포함합니다.

using namespace std;

class RecipeDatabase
{
private:
    vector<Recipe> recipes;
    string filename;

    // --- Private Helper Functions ---

    /*
     * string trim(const string &str): 문자열의 시작과 끝에 있는 공백(스페이스, 탭, 개행 등)을 제거합니다.
     * param str 원본 문자열
     * string::find_first_not_of와 string::find_last_not_of를 사용하여 공백이 아닌 첫 문자와 마지막 문자의 위치를 찾습니다.
     * return 공백이 제거된 새로운 문자열
     */
    // 문자열 좌우 공백 제거 헬퍼 함수
    string trim(const string &str)
    {
        size_t first = str.find_first_not_of(" \t\n\r");
        if (string::npos == first)
            return str;
        size_t last = str.find_last_not_of(" \t\n\r");
        return str.substr(first, (last - first + 1));
    }

    /*
     * vector<string> split(const string &s, char delimiter): 주어진 구분자(delimiter)를 기준으로 문자열을 분리하여 vector<string>에 담아 반환합니다.
     * param s 분리할 원본 문자열
     * param delimiter 구분자 문자
     * istringstream을 사용해 문자열을 스트림으로 만들고, getline 함수로 구분자가 나올 때까지 문자열을 읽어와 벡터에 추가합니다.
     * return 분리된 문자열들의 벡터
    */
    // 문자열 분리 헬퍼 함수
    vector<string> split(const string &s, char delimiter)
    {
        vector<string> tokens;
        string token;
        istringstream tokenStream(s);
        while (getline(tokenStream, token, delimiter))
        {
            tokens.push_back(trim(token));
        }
        return tokens;
    }


    /*
     * static string toLower(const string &str): 문자열 전체를 소문자로 변환합니다. 대소문자 구분 없는 검색을 위해 사용됩니다.
     * param str 원본 문자열
     * details: algorithm 헤더의 transform 함수를 사용하여 문자열의 각 문자에 tolower 함수를 적용합니다.
     * return 모두 소문자로 변환된 새로운 문자열
     */
    static string toLower(const string &str)
    {
        string lower_str = str;
        transform(lower_str.begin(), lower_str.end(), lower_str.begin(),
                  [](unsigned char c)
                  { return tolower(c); });
        return lower_str;
    }

    /*
     * static Difficulty stringToDifficulty(const string &s): 난이도를 나타내는 문자열("A", "B", "C")을 Difficulty 열거형(enum)으로 변환합니다.
     * param s 난이도 문자열
     * details: static 함수인 이유: 특정 RecipeDatabase 객체의 상태(recipes, filename)와 무관하게 동작하므로 static으로 선언하는 것이 효율적입니다.
     * return 변환된 Difficulty enum 값
     */
    static Difficulty stringToDifficulty(const string &s)
    {
        string lower_s = toLower(s);
        if (lower_s == "a")
            return Difficulty::A;
        if (lower_s == "b")
            return Difficulty::B;
        if (lower_s == "c")
            return Difficulty::C;
        return Difficulty::A; // 기본값
    }

    // Difficulty enum을 'Grade' 문자열로 변환
    static string difficultyToString(Difficulty d)
    {
        switch (d)
        {
        case Difficulty::A:
            return "A";
        case Difficulty::B:
            return "B";
        case Difficulty::C:
            return "C";
        default:
            return "?";
        }
    }

public:
    /**
     * vector<Ingredient> parseIngredients(const string& value): "이름|양|단위,이름|양|단위" 형식의 재료 문자열을 파싱하여 vector<Ingredient> 객체로 변환합니다.
     * param value 파싱할 재료 정보 문자열
     * details: 먼저 ','를 기준으로 각 재료를 분리하고, 다시 '|'를 기준으로 이름, 양, 단위를 분리하여 Ingredient 객체를 생성합니다.
     * stod (string to double) 함수를 사용하며, 숫자로 변환할 수 없는 경우를 대비해 try-catch로 예외 처리를 합니다.
     * return 파싱된 재료 객체들의 벡터
     */
    // Ingredients 문자열을 파싱하여 vector<Ingredient>로 반환
    vector<Ingredient> parseIngredients(const string& value) {
        vector<string> ingredientStrings = split(value, ',');
        vector<Ingredient> newIngredients;
        for (const string& ingStr : ingredientStrings) {
            vector<string> parts = split(ingStr, '|');
            if (parts.size() == 3) {
                Ingredient ing;
                ing.name = parts[0];
                try {
                    ing.quantity = stod(parts[1]);
                } catch (const std::exception&) {
                    ing.quantity = 0;
                }
                ing.unit = parts[2];
                newIngredients.push_back(ing);
            }
        }
        return newIngredients;
    }
    
    
    // --- 파일 입출력 ---

    /*
     * bool loadFromFile(const string& filename): 지정된 파일로부터 레시피 데이터를 읽어와 `recipes` 벡터를 채웁니다.
     * param filename 읽어올 파일의 경로 및 이름
     * details: 멀티라인으로 구성된 레시피 형식을 파싱합니다. 'Recipe Procedure' 섹션을 읽을 때는 상태(isReadingProcedure)를 관리하며,
     * 'Recipe name' 키워드가 나오면 이전까지 읽은 정보를 바탕으로 레시피 객체를 생성하고 저장합니다.
     * return 파일을 성공적으로 읽어왔으면 true, 실패했으면 false를 반환합니다.
     */
    // 새로운 멀티라인 포맷을 처리하는 파싱 함수
    // 새로운 Ingredient 파싱 로직 적용 
    bool loadFromFile(const string& filename) {
        this->filename = filename;
        ifstream file(filename);
        if (!file.is_open()) return false;
        recipes.clear();

        string line, currentTitle, currentProcedure;
        int currentTime = 0;
        vector<Ingredient> currentIngredient; // vector<Ingredient> 사용
        Difficulty currentDifficulty = Difficulty::A;
        bool isReadingProcedure = false;

        // 람다(lambda) 함수: 현재까지 파싱된 정보로 레시피 객체를 생성하고 벡터에 추가하는 로직을 캡슐화합니다.
        auto createAndStoreRecipe = [&]() {
            if (!currentTitle.empty()) {
                recipes.emplace_back(currentTitle, trim(currentProcedure), currentTime, currentIngredient, currentDifficulty);
                currentTitle.clear();
                currentProcedure.clear();
                currentIngredient.clear();
            }
        };

        // fstream 헤더의 getline 함수: 파일에서 한 줄씩 읽어옵니다.
        while (getline(file, line)) {
            string trimmed_line = trim(line);
            if (isReadingProcedure) {
                size_t colon_pos = trimmed_line.find(':');
                if (colon_pos != string::npos) {
                    string key = trim(trimmed_line.substr(0, colon_pos));
                    if (key == "Time" || key == "Ingredients" || key == "Grade" || key == "Recipe name") {
                        isReadingProcedure = false;
                    }
                }
            }
            if (isReadingProcedure) {
                 if (!trimmed_line.empty()) currentProcedure += trimmed_line + "\n";
                 continue;
            }
            size_t colon_pos = trimmed_line.find(':');
            if (colon_pos != string::npos) {
                string key = trim(trimmed_line.substr(0, colon_pos));
                string value = trim(trimmed_line.substr(colon_pos + 1));

                if (key == "Recipe name") {
                    createAndStoreRecipe();
                    currentTitle = value;
                } else if (key == "Recipe Procedure") {
                    isReadingProcedure = true;
                } else if (key == "Time") {
                    currentTime = stoi(value);
                } else if (key == "Ingredients") {
                    currentIngredient = parseIngredients(value); // 새로운 파싱 로직 호출
                } else if (key == "Grade") {
                    currentDifficulty = stringToDifficulty(value);
                }
            }
        }
        createAndStoreRecipe();
        file.close();
        return true;
    }

    /*
     * bool saveToFile() const: 현재 `recipes` 벡터에 있는 모든 레시피 데이터를 파일에 저장합니다.
     * return 파일 저장 성공 시 true, 실패 시 false
     */
    
    // 'Grade' 형식으로 저장하는 함수
    bool saveToFile() const {
        ofstream file(filename);
        if (!file.is_open()) return false;
        for (const auto& recipe : recipes) {
            file << "Recipe name: " << recipe.getTitle() << "\n";
            file << "Recipe Procedure:\n" << recipe.getProcedure() << "\n";
            file << "Time: " << recipe.getTime() << "\n";
            
            file << "Ingredients: ";
            const auto& ingredients = recipe.getIngredient();
            for (size_t j = 0; j < ingredients.size(); ++j) {
                const auto& ing = ingredients[j];
                file << ing.name << "|" << ing.quantity << "|" << ing.unit;
                if (j < ingredients.size() - 1) file << ", ";
            }
            file << "\n";
            file << "Grade: " << difficultyToString(recipe.getDifficulty()) << "\n\n";
        }
        file.close();
        return true;
    }


    // --- 5대 핵심 기능 ---

    /*
     * void insertRecipe(): 사용자로부터 새로운 레시피 정보를 입력받아 데이터베이스에 추가합니다.
     * details: 여러 줄로 된 조리 절차를 입력받기 위해 빈 줄이 입력될 때까지 getline을 반복 사용합니다.
     */
    void insertRecipe() {
        string title, procedure, timeStr, ingredientsLine, gradeStr;
        cout << "Enter recipe title: ";
        getline(cin, title);
        cout << "Enter recipe procedure (end with an empty line):\n";
        string proc_line;
        while(getline(cin, proc_line) && !proc_line.empty()) {
            procedure += proc_line + "\n";
        }
        cout << "Enter cooking time (minutes): ";
        getline(cin, timeStr);
        cout << "Enter ingredients (e.g., flour|1|cup, egg|2|ea): ";
        getline(cin, ingredientsLine);
        cout << "Enter grade (A, B, C): ";
        getline(cin, gradeStr);

        recipes.emplace_back(
            title, 
            procedure, 
            stoi(timeStr), 
            parseIngredients(ingredientsLine), // parseIngredients 헬퍼 사용
            stringToDifficulty(gradeStr)
        );
        cout << "\nRecipe '" << title << "' added successfully!" << endl;
    }

    /*
     * void searchRecipe() const: 사용자로부터 키워드를 입력받아 레시피 제목 또는 재료 이름에 해당 키워드가 포함된 모든 레시피를 검색하고 출력합니다.
     * details: 대소문자를 구분하지 않고 검색하기 위해 toLower 헬퍼 함수를 사용합니다.
     */
    void searchRecipe() const {
        cout << "Enter keyword to search for (case-insensitive): ";
        string keyword;
        getline(cin, keyword);
        string lowerKeyword = toLower(keyword);

        bool foundAny = false;
        cout << "\n--- Search Results ---\n";

        for (const auto& recipe : recipes) {
            // string::npos: find 함수가 문자열을 찾지 못했을 때 반환하는 특별한 값
            bool foundInThisRecipe = (toLower(recipe.getTitle()).find(lowerKeyword) != string::npos);

            if (!foundInThisRecipe) {
                for (const auto& ingredient : recipe.getIngredient()) {
                    if (toLower(ingredient.name).find(lowerKeyword) != string::npos) {
                        foundInThisRecipe = true;
                        break; // 재료에서 찾았으면 더 이상 탐색할 필요 없음
                    }
                }
            }

            if (foundInThisRecipe) {
                recipe.display();
                foundAny = true;
            }
        }
        if (!foundAny) {
            cout << "No recipes found matching '" << keyword << "'.\n";
        }
    }

    /*
     * void deleteRecipe(): 사용자로부터 제목을 입력받아 일치하는 레시피를 데이터베이스에서 삭제합니다.
     * details: C++의 'Erase-Remove Idiom'을 사용합니다.
     * 1. remove_if: 삭제할 조건에 맞는 원소들을 벡터의 뒤쪽으로 모두 옮기고, 유효한 원소들의 끝을 가리키는 반복자(iterator)를 반환합니다. (실제 삭제는 아님)
     * 2. erase: remove_if가 반환한 위치부터 벡터의 실제 끝까지의 원소들을 한 번에 삭제합니다.
     */
    void deleteRecipe()
    {
        cout << "Enter title of the recipe to delete: ";
        string title;
        getline(cin, title);

        // 람다 함수 `[&](...) { ... }`: 삭제할 조건을 정의하는 함수 객체를 즉석에서 만듭니다. `[&]`는 외부 변수(title)를 참조로 캡처합니다.
        auto it = remove_if(recipes.begin(), recipes.end(), [&](const Recipe &recipe)
                            { return recipe.getTitle() == title; });

        if (it != recipes.end())
        {
            recipes.erase(it, recipes.end());
            cout << "Recipe '" << title << "' deleted successfully." << endl;
        }
        else
        {
            cout << "Recipe not found." << endl;
        }
    }

    /**
     * void sortRecipe(): `recipes` 벡터를 레시피 제목의 가나다/알파벳 순으로 정렬합니다.
     * details: algorithm 헤더의 sort 함수를 사용합니다. 정렬 기준은 람다 함수로 제공합니다.
     */
    void sortRecipe()
    {
        sort(recipes.begin(), recipes.end(), [](const Recipe &a, const Recipe &b)
             { return a.getTitle() < b.getTitle(); });
        cout << "Recipes have been sorted by title." << endl;
    }

    /*
     * void editRecipe(): 사용자로부터 제목을 입력받아 특정 레시피의 정보를 수정합니다.
     * details: cin으로 숫자를 입력받은 후 getline으로 문자열을 입력받기 전에 입력 버퍼에 남아있는 개행 문자를 제거해야 합니다.
     * `cin.ignore(numeric_limits<streamsize>::max(), '\n');` 코드가 이 역할을 합니다.
     */
    void editRecipe()
{
    cout << "Enter title of the recipe to edit: ";
    string title;
    getline(cin, title);

    for (auto &recipe : recipes)
    {
        if (toLower(recipe.getTitle()) == toLower(title)) // 대소문자 구분 없이 검색
        {
            cout << "Recipe found. Which part do you want to edit?" << endl;
            cout << "1. Title\n2. Procedure\n3. Time\n4. Ingredients\n5. Grade\n> ";
            int choice;
            cin >> choice;
            cin.ignore(numeric_limits<streamsize>::max(), '\n');

            switch (choice)
            {
            case 1: {
                cout << "Enter new title: ";
                string input;
                getline(cin, input);
                recipe.setTitle(input);
                break;
            }

            case 2: {
                cout << "Enter new procedure (end with an empty line):\n";
                string newProcedure;
                string proc_line;
                while (getline(cin, proc_line) && !proc_line.empty())
                {
                    newProcedure += proc_line + "\n";
                }
                recipe.setProcedure(trim(newProcedure)); // 끝에 불필요한 공백 제거
                break;
            }

            case 3: {
                cout << "Enter new time (minutes): ";
                string input;
                getline(cin, input);
                try {
                    recipe.setTime(stoi(input)); // 문자열을 숫자로 변환
                } catch (const std::exception& e) {
                    cout << "Invalid number format. Time not updated." << endl;
                }
                break;
            }

            case 4: {
                cout << "Enter new ingredients (e.g., flour|1|cup, egg|2|ea): ";
                string input;
                getline(cin, input);
                recipe.setIngredient(parseIngredients(input));
                break;
            }

            case 5: {
                cout << "Enter new grade (A, B, C): ";
                string input;
                getline(cin, input);
                recipe.setDifficulty(stringToDifficulty(input));
                break;
            }

            default:
                cout << "Invalid choice. No changes made." << endl;
                return;
            }
            cout << "Recipe updated successfully." << endl;
            return;
        }
    }
    cout << "Recipe not found." << endl;
}

    void displayAll() const
    {
        if (recipes.empty())
        {
            cout << "No recipes in the database." << endl;
            return;
        }
        for (const auto &recipe : recipes)
        {
            recipe.display();
        }
    }

    // Meal에서 RecipeDatabase연동
    // Meal이 DB에서 레시피를 찾으려면 내부 vector에 접근해야 해서
    const vector<Recipe> &getRecipes() const { return recipes; }

    // 난이도별 레시피 검색 함수 (추천 시스템용)
    vector<string> getRecipesByDifficultyLevel(char maxLevel) const
    {
        vector<string> result;

        for (const auto &recipe : recipes)
        {
            Difficulty recipeDiff = recipe.getDifficulty();

            // 선택된 난이도 이하의 레시피만 포함
            bool shouldInclude = false;

            if (maxLevel == 'A')
            {
                // A 선택: A, B, C 모든 난이도 포함
                shouldInclude = true;
            }
            else if (maxLevel == 'B')
            {
                // B 선택: B, C 난이도만 포함 (A 제외)
                shouldInclude = (recipeDiff == Difficulty::B || recipeDiff == Difficulty::C);
            }
            else if (maxLevel == 'C')
            {
                // C 선택: C 난이도만 포함
                shouldInclude = (recipeDiff == Difficulty::C);
            }

            if (shouldInclude)
            {
                result.push_back(recipe.getTitle());
            }
        }

        return result;
    }
};

#endif // RECIPE_DATABASE_H