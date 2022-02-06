from .joseki import josekis
from . import AI
import random
import copy

c4 = (2, 3)
d3 = (3, 2)
e6 = (4, 5)
f5 = (5, 4)


def getRandomAction(board, moves):
    """ランダムなアクションを返却する"""
    return random.choice(moves)


class JosekiAI:
    """定石を打つAIクラス"""
    SIZE = 8
    kifu = ''
    prev_board = None

    def setAction(self, board, moves):
        """初期化"""
        self.board = board
        self.moves = moves

    def regularize(self, pos):
        """第一手によって座標の正規化を行う"""
        if self.first == c4:
            return self.SIZE-1-pos[0],  self.SIZE-1-pos[1]
        if self.first == d3:
            return self.SIZE-1-pos[1],  self.SIZE-1-pos[0]
        if self.first == e6:
            return pos[1], pos[0]
        if self.first == f5:
            return pos

    def irregularize(self, pos):
        """正規座標を元に戻す
        regularizeと同じとなる"""
        return self.regularize(pos)

    def posToRegularStr(self, pos):
        """座標を正規座標の文字にする"""
        rpos = self.regularize(pos)
        return 'abcdefgh'[rpos[0]]+str(rpos[1]+1)

    def regularStrToPos(self, regStr):
        """正規座標の文字を座標にする"""
        return self.irregularize(('abcdefgh'.index(regStr[0]), int(regStr[1])-1))

    def addPrevKifu(self):
        """パスが無いことを前提に直前の棋譜を追加する"""
        for i in range(self.SIZE):
            for j in range(self.SIZE):
                if self.prev_board[i][j] == 0 and self.board[i][j] == -1:
                    self.kifu += self.posToRegularStr((j, i))
                    return
        assert False

    def getJoseki(self):
        """棋譜から定石を得る
        無いならNoneを返す"""
        print('kifu', self.kifu)
        for joseki in josekis:
            if joseki.startswith(self.kifu) and len(joseki) > len(self.kifu):
                print('Use Joseki', joseki)
                moveRegStr = joseki[len(self.kifu):len(self.kifu)+2]
                move = self.regularStrToPos(moveRegStr)
                return move
        return None

    def getAction(self):
        """次の手を得る"""

        stones = 0
        for line in self.board:
            stones += line.count(1) + line.count(-1)
        self.turn = stones - 4 + 1

        print('turn', self.turn)

        if self.turn == 1:
            # 1ターン目なら自分がf5に置く
            self.first = f5

        if self.turn == 2:
            # 2ターン目なら相手は4箇所のいずれかに置いていたのでそれをfirstに代入
            fs = [c4, d3, e6, f5]
            for f in fs:
                if self.board[f[1]][f[0]] == -1:
                    self.first = f

        if self.turn == 2:
            # 2ターン目なら相手の置いた箇所に依らず棋譜上はf5に置いた扱い
            self.kifu += 'f5'
        if self.turn > 2:
            self.addPrevKifu()

        # 二次元配列は[:]ではコピーできない
        self.prev_board = copy.deepcopy(self.board)

        j = self.getJoseki()
        if j:
            self.kifu += self.posToRegularStr(j)
            return list(j)

        print('No Joseki')

        return None


jai = JosekiAI()
USE_JOSEKI = True


def getAction(board, moves):
    """アクションを返却"""
    global jai, USE_JOSEKI
    print()
    print('moves', moves)
    if USE_JOSEKI:
        jai.setAction(board, moves)
        action = jai.getAction()
        if action is None:
            USE_JOSEKI = False
    if not USE_JOSEKI:
        action = list(AI.getAction(board, moves))
    if action not in moves:
        print(f'action {action} not in moves {moves}')
        exit(1)
    return action
