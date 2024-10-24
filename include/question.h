#ifndef QUESTION
#define QUESTION

struct question {
  char *name;
  unsigned short type;
  unsigned short clas;
};

void question_init(struct question *question, char *name, unsigned short type,
                   unsigned short clas);
char *dezerialize_question(struct question *question);

#endif // !QUESTION
#define QUESTIO
