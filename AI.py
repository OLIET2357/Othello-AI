import os
import random
from time import time
import threading
import subprocess

SCORE_PATH = os.path.dirname(__file__)+'\\'+'score.exe'


def getAction(board, moves):
    if len(moves) == 1:
        print('Only one move, immediately return')
        return moves[0]

    stones = 0
    for e in board:
        for f in e:
            if f != 0:
                stones += 1
    turn = stones-4+1

    TIMEOUT = 10

    SIZE = 8

    i = '4 8 16 12\n'
    i += str(SIZE)+'\n'
    for f in [' '.join(map(str, e)) for e in board]:
        i += f+'\n'

    rets = []
    ts = []
    for move in moves:
        def ai(i, move):
            i += f'{move[0]} {move[1]}\n'
            # print(i)
            try:
                start = time()
                result = subprocess.run(
                    (SCORE_PATH,),  input=i.encode(), stdout=subprocess.PIPE, timeout=TIMEOUT)
                end = time()
                print(
                    f'turn:{turn} time:{end-start}')
            except subprocess.TimeoutExpired:
                print(
                    f'turn:{turn} time:TIMEOUT (>{TIMEOUT}s)')
                return

            if result.returncode == 1:
                print('Error')
                print()
                print(i)
                return
            score = int(result.stdout.decode().replace('\n', ''))
            rets.append((score, move))

        if not os.path.exists(SCORE_PATH):
            print(f'{SCORE_PATH} not found')
            exit(1)

        t = threading.Thread(target=ai, args=(i, move))
        t.start()
        ts.append(t)

    for t in ts:
        t.join()

    print(rets)

    if len(rets) == 0:
        print('No moves')
        return random.choice(moves)

    # return max(rets)[1]

    # return randomly which has the maximum score
    return random.choice([ret[1] for ret in rets if ret[0] == max(rets)[0]])


assert __name__ != '__main__', "Don't run this file"
