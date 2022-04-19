#include "level.h"

Level::Level(string _title, vector<string> _lesson, vector<string> _validTreatments,
             vector<string> _symptoms, vector<QPixmap> _patientStages, bool _inOrder)
{
    title = _title;
    lesson = _lesson;
    validTreatments = _validTreatments;
    symptoms = _symptoms;
    patientStages = _patientStages;
    inOrder = _inOrder;
}
