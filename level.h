#ifndef LEVEL_H
#define LEVEL_H

#include <vector>
#include <string>
#include <QPixmap>

using namespace std;


class Level
{
public:
    Level(string _title, vector<string> _lesson, vector<string> _validTreatments,
          vector<string> _symptoms, vector<QPixmap> _patientStages, bool _inOrder);
private:
    string title;
    vector<string> lesson;
    vector<string> validTreatments; //ordered
    vector<string> symptoms;
    vector<QPixmap> patientStages; //ordered
    bool inOrder;
};

#endif // LEVEL_H
