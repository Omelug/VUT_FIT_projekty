def gen_quiz(qpool, *indexes, altcodes='ABCDEF', quiz=None):
    if quiz is None:
        quiz = []
    for index in indexes:
        try:
            quiz.append((qpool[index][0], [x + ": " + y for x, y in list(zip(altcodes, qpool[index][1]))]))
        except Exception as exeption:
            print(f"Ignoring index {index} - {exeption}")
    return quiz
