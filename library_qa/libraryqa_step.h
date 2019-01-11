#ifndef LIBRARYQA_STEP_H
#define LIBRARYQA_STEP_H


class LibraryQA_Step
{
public:
    LibraryQA_Step();

    /**
     * @brief The Severity enum
     */
    enum Severity {
      Warning,
      Error,
      Passed
    };


};

#endif // LIBRARYQA_STEP_H
