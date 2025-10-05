
#ifndef RECIPE_DATABASE_H
#define RECIPE_DATABASE_H

#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <limits>
#include "Recipe.h" // Recipe 클래스 헤더 포함

using namespace std;

class RecipeDatabase
{
private:
    vector<Recipe> recipes;
    string filename;

    // --- Private Helper Functions ---

    // 문자열 좌우 공백 제거 헬퍼 함수
    string trim(const string &str)
    {
        size_t first = str.find_first_not_of(" \t\n\r");
        if (string::npos == first)
            return str;
        size_t last = str.find_last_not_of(" \t\n\r");
        return str.substr(first, (last - first + 1));
    }

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

    static string toLower(const string &str)
    {
        string lower_str = str;
        transform(lower_str.begin(), lower_str.end(), lower_str.begin(),
                  [](unsigned char c)
                  { return tolower(c); });
        return lower_str;
    }

    // Difficulty enum을 문자열로 변환하는 헬퍼 함수
    // 클래스 상태에 접근하지 않으므로 static으로 선언하는 것이 좋음
    // 'Grade'를 Difficulty enum으로 변환
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

    // [수정됨] Ingredients 문자열을 파싱하여 vector<Ingredient>로 반환
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

    // 새로운 멀티라인 포맷을 처리하는 파싱 함수
    // [수정됨] 새로운 Ingredient 파싱 로직 적용 10-05 pm10:45
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

        auto createAndStoreRecipe = [&]() {
            if (!currentTitle.empty()) {
                recipes.emplace_back(currentTitle, trim(currentProcedure), currentTime, currentIngredient, currentDifficulty);
                currentTitle.clear();
                currentProcedure.clear();
                currentIngredient.clear();
            }
        };

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

    // [수정] 'Grade' 형식으로 저장하는 함수
    // [수정됨] 새로운 Ingredient 형식으로 저장 10-05 pm10:45
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

    // [수정] 'Grade'를 입력받도록 변경
    // [수정됨] '이름|양|단위' 형식으로 입력받도록 변경 10-05 pm10:45
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

    // [수정] 대소문자 미구분 검색 기능
    // [수정됨] Ingredient의 name 필드에서 검색 가능하도록 변경 10-05 pm10:45
    void searchRecipe() const {
        cout << "Enter keyword to search for (case-insensitive): ";
        string keyword;
        getline(cin, keyword);
        string lowerKeyword = toLower(keyword);

        bool foundAny = false;
        cout << "\n--- Search Results ---\n";

        for (const auto& recipe : recipes) {
            bool foundInThisRecipe = (toLower(recipe.getTitle()).find(lowerKeyword) != string::npos);

            if (!foundInThisRecipe) {
                for (const auto& ingredient : recipe.getIngredient()) {
                    if (toLower(ingredient.name).find(lowerKeyword) != string::npos) {
                        foundInThisRecipe = true;
                        break;
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

    void deleteRecipe()
    {
        cout << "Enter title of the recipe to delete: ";
        string title;
        getline(cin, title);

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

    void sortRecipe()
    {
        sort(recipes.begin(), recipes.end(), [](const Recipe &a, const Recipe &b)
             { return a.getTitle() < b.getTitle(); });
        cout << "Recipes have been sorted by title." << endl;
    }

    // [수정됨] '이름|양|단위' 형식으로 수정하도록 변경 10-05 pm10:45
    void editRecipe() {
        cout << "Enter title of the recipe to edit: ";
        string title;
        getline(cin, title);

        for (auto& recipe : recipes) {
            if (recipe.getTitle() == title) {
                cout << "Recipe found. Which part to edit?" << endl;
                cout << "1. Title\n2. Procedure\n3. Time\n4. Ingredients\n5. Grade\n> ";
                int choice;
                cin >> choice;
                cin.ignore(numeric_limits<streamsize>::max(), '\n');

                switch (choice) {
                    case 1: { /* ... */ break; }
                    case 2: { /* ... */ break; }
                    case 3: { /* ... */ break; }
                    case 4: {
                        cout << "Enter new ingredients (e.g., flour|1|cup, egg|2|ea): ";
                        string input;
                        getline(cin, input);
                        recipe.setIngredient(parseIngredients(input));
                        break;
                    }
                    case 5: { /* ... */ break; }
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