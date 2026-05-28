#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <algorithm>
#include <map>

using namespace std;

struct Student {
    string id;
    int age;
    string gender;
    double study_hours;
    double social_media;
    double netflix;
    string part_time_job;
    double attendance;
    double sleep_hours;
    string diet_quality;
    int exercise;
    string parent_edu;
    string internet_quality;
    int mental_health;
    string extracurricular;
    double exam_score;

    int gender_code;
    int part_time_code;
    int diet_code;
    int parent_edu_code;
    int internet_code;
    int extra_code;
};

Student parseLine(const string& line) {
    Student s = {};
    stringstream ss(line);
    string field;
    vector<string> fields;
    while (getline(ss, field, ','))
        fields.push_back(field);
    if (fields.size() < 16) return s;

    s.id                = fields[0];
    s.age               = stoi(fields[1]);
    s.gender            = fields[2];
    s.study_hours       = stod(fields[3]);
    s.social_media      = stod(fields[4]);
    s.netflix           = stod(fields[5]);
    s.part_time_job     = fields[6];
    s.attendance        = stod(fields[7]);
    s.sleep_hours       = stod(fields[8]);
    s.diet_quality      = fields[9];
    s.exercise          = stoi(fields[10]);
    s.parent_edu        = fields[11];
    s.internet_quality  = fields[12];
    s.mental_health     = stoi(fields[13]);
    s.extracurricular   = fields[14];
    s.exam_score        = stod(fields[15]);
    return s;
}

vector<Student> loadData(const string& filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Ошибка: не могу открыть " << filename << endl;
        return {};
    }
    vector<Student> students;
    string line;
    bool first = true;
    while (getline(file, line)) {
        if (first) { first = false; continue; }
        Student s = parseLine(line);
        if (!s.id.empty()) students.push_back(s);
    }
    file.close();

    map<string, int> freq;
    for (const auto& st : students)
        if (!st.parent_edu.empty() && st.parent_edu != "NA")
            freq[st.parent_edu]++;

    string mode;
    int maxf = 0;
    for (auto& p : freq)
        if (p.second > maxf) { maxf = p.second; mode = p.first; }

    for (auto& st : students)
        if (st.parent_edu.empty() || st.parent_edu == "NA")
            st.parent_edu = mode;

    cout << "Заполнено пропусков parental_education_level значением: " << mode << endl;
    return students;
}

void encode(vector<Student>& students) {
    for (auto& s : students) {
        if (s.gender == "Female") s.gender_code = 0;
        else if (s.gender == "Male") s.gender_code = 1;
        else s.gender_code = 2;

        s.part_time_code = (s.part_time_job == "Yes") ? 1 : 0;

        if (s.diet_quality == "Poor") s.diet_code = 1;
        else if (s.diet_quality == "Fair") s.diet_code = 2;
        else s.diet_code = 3;

        if (s.parent_edu == "None") s.parent_edu_code = 0;
        else if (s.parent_edu == "High School") s.parent_edu_code = 1;
        else if (s.parent_edu == "Bachelor") s.parent_edu_code = 2;
        else s.parent_edu_code = 3;

        if (s.internet_quality == "Poor") s.internet_code = 1;
        else if (s.internet_quality == "Average") s.internet_code = 2;
        else s.internet_code = 3;

        s.extra_code = (s.extracurricular == "Yes") ? 1 : 0;
    }
    cout << "Категории закодированы.\n";
}

void clean(vector<Student>& students) {
    int before = students.size();
    students.erase(remove_if(students.begin(), students.end(),
        [](const Student& s) { return s.study_hours == 0.0; }),
        students.end());
    cout << "Удалено строк с study_hours == 0: " << before - students.size() << endl;
    cout << "Осталось записей: " << students.size() << endl;
}

void advancedStats(const vector<Student>& students) {
    map<string, pair<double, int>> studyGroups;
    for (const auto& s : students) {
        string group;
        if (s.study_hours < 2) group = "0-2 часа";
        else if (s.study_hours < 4) group = "2-4 часа";
        else if (s.study_hours < 6) group = "4-6 часов";
        else group = "более 6 часов";
        studyGroups[group].first += s.exam_score;
        studyGroups[group].second++;
    }

    cout << "\nСредний балл по времени учёбы\n";
    for (const auto& [name, data] : studyGroups) {
        double avg = data.first / data.second;
        cout << name << ": " << avg << " баллов (n=" << data.second << ")\n";
    }

    map<string, pair<double, int>> dietGroups;
    for (const auto& s : students) {
        string diet;
        if (s.diet_code == 1) diet = "Poor";
        else if (s.diet_code == 2) diet = "Fair";
        else diet = "Good";
        dietGroups[diet].first += s.exam_score;
        dietGroups[diet].second++;
    }

    cout << "\n Средний балл по качеству питания \n";
    for (const auto& [name, data] : dietGroups) {
        double avg = data.first / data.second;
        cout << name << ": " << avg << " баллов (n=" << data.second << ")\n";
    }

    map<string, pair<double, int>> genderGroups;
    for (const auto& s : students) {
        string g;
        if (s.gender_code == 0) g = "Female";
        else if (s.gender_code == 1) g = "Male";
        else g = "Other";
        genderGroups[g].first += s.exam_score;
        genderGroups[g].second++;
    }

    cout << "\n Средний балл по полу \n";
    for (const auto& [name, data] : genderGroups) {
        double avg = data.first / data.second;
        cout << name << ": " << avg << " баллов (n=" << data.second << ")\n";
    }

    map<string, pair<double, int>> sleepGroups;
    for (const auto& s : students) {
        string group;
        if (s.sleep_hours < 6) group = "менее 6 часов";
        else if (s.sleep_hours < 7) group = "6-7 часов";
        else if (s.sleep_hours < 8) group = "7-8 часов";
        else group = "более 8 часов";
        sleepGroups[group].first += s.exam_score;
        sleepGroups[group].second++;
    }

    cout << "\n Средний балл по времени сна \n";
    for (const auto& [name, data] : sleepGroups) {
        double avg = data.first / data.second;
        cout << name << ": " << avg << " баллов (n=" << data.second << ")\n";
    }

    map<string, pair<double, int>> socialGroups;
    for (const auto& s : students) {
        string group;
        if (s.social_media < 2) group = "менее 2 часов";
        else if (s.social_media < 4) group = "2-4 часа";
        else group = "более 4 часов";
        socialGroups[group].first += s.exam_score;
        socialGroups[group].second++;
    }

    cout << "\n Средний балл по времени в соцсетях \n";
    for (const auto& [name, data] : socialGroups) {
        double avg = data.first / data.second;
        cout << name << ": " << avg << " баллов (n=" << data.second << ")\n";
    }
}

void save(const vector<Student>& students, const string& filename) {
    ofstream out(filename);
    out << "age,study_hours,social_media,netflix,attendance,sleep_hours,exercise,"
        << "mental_health,exam_score,gender_code,part_time_code,diet_code,"
        << "parent_edu_code,internet_code,extra_code\n";
    for (const auto& s : students) {
        out << s.age << ","
            << s.study_hours << ","
            << s.social_media << ","
            << s.netflix << ","
            << s.attendance << ","
            << s.sleep_hours << ","
            << s.exercise << ","
            << s.mental_health << ","
            << s.exam_score << ","
            << s.gender_code << ","
            << s.part_time_code << ","
            << s.diet_code << ","
            << s.parent_edu_code << ","
            << s.internet_code << ","
            << s.extra_code << "\n";
    }
    cout << "\nОчищенные данные сохранены в " << filename << endl;
}

int main() {
    string input = "cpp/data/student_habits_performance.csv";
    string output = "cpp/cleaned_students.csv";

    cout << "Загрузка данных...\n";
    vector<Student> students = loadData(input);
    if (students.empty()) return 1;

    encode(students);
    clean(students);
    advancedStats(students);
    save(students, output);

    return 0;
}