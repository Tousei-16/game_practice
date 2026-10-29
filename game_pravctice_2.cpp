#include <iostream>
#include <stdio.h>
#include <ctime>
#include <cstdlib>
#include <sstream>
#include <string>

class Character // スーパークラスを定義し、プレイヤー・敵クラスの共通部分のメンバ変数・メンバ関数を定義していく。
{
public:
    int Level;
    int hp;
    int attack;
    int speed;
    Character(int lvl, int base_hp, int base_attack, int base_speed) : Level(lvl) // ステータス生成
    {
        hp = base_hp + 10 * lvl;
        attack = base_attack + 2 * lvl;
        speed = base_speed + lvl;
    }

    virtual ~Character() = default;

    virtual void Attack(Character &target) // 攻撃によって、引数のメンバ変数hpを減らす。
    {
        printf("\n attack\n");
        target.hp -= attack;
        printf("%d damage\n", attack);
    }
    virtual void Heal() = 0; // プレイヤー・敵クラスによって、動きが違うため、宣言のみ。
    virtual int is_Alive()   // プレイヤー・敵の生死判断の関数
    {
        if ((hp > 0))
        {
            return 0;
        }
        else
        {
            return -1;
        }
    }
    virtual void check() = 0; // プレイヤー・敵クラスによって、動きが違うため、宣言のみ。
};

class Player : public Character // サブクラスを定義する。
{
public:
    int defence;
    int experience;
    void Levelup();
    void gainexp(int exp);
    Player(int lvl) : Character(lvl, 50, 10, 40), defence(5 + lvl), experience(0) {} // プレイヤーのステータス生成

    void Attack(Character &enemy) // プレイヤーの攻撃によって、敵HPが攻撃力分減る。
    {
        printf("\nPlayer attack\n");
        enemy.hp -= attack;
        printf("%d damage\n", attack);
    };
    void Defence() // プレイヤーの操作を表示する。
    {
        printf("\nPlayer defence\n");
    };
    void Heal() // プレイヤーは、MaxHPの半分回復できる。
    {
        int maxHp = 50 + 10 * Level;
        int oldHp = hp;

        hp += maxHp / 2;

        if (hp > maxHp) // 回復で上限オーバーの場合、HPが上限値になる。
        {
            hp = maxHp;
        }
        int recoveredHp = hp - oldHp;
        printf("\nPlayer recovers \n");
        printf("recover %d Hp\n", recoveredHp);
    };
    void check() // プレイヤー情報の表示
    {
        printf("\nplayer\n");
        if (hp <= 0)
        {
            printf("HP : 0\n");
        }
        else
        {
            printf("HP : %d\n", hp);
        }
        printf("Lv : %d\n", Level);
        printf("exp : %d\n", experience);
    }
};

class Enemy : public Character //
{
public:
    Enemy(int lvl) : Character(lvl, 40, 8, 4) {} // 敵ステータスの表示
    void Attack(Character &player)               // 敵の攻撃によって、プレイヤーHPが攻撃力分減る。
    {
        printf("\nEnemy attack\n");
        player.hp -= attack;
        printf("Damage %d\n", attack);
    };

    void Heal() // 敵は、MaxHPの5分の1回復できる。
    {
        int maxHp = 40 + 10 * Level;
        int oldHp = hp;

        hp += maxHp / 5;

        // 最大HPを超えないようにする
        if (hp > maxHp)
        {
            hp = maxHp;
        }

        // 実際に回復した量を表示
        int recoveredHp = hp - oldHp;

        printf("\nEnemy recovers\n");
        printf("recover %d HP\n", recoveredHp);
    };

    void DefencedAttack(Player &player) // プレイヤーが防御しているとき、敵の攻撃の処理
    {
        printf("\nEnemy attack\n");
        if ((attack - player.defence) >= 0) // 敵攻撃力がプレイヤーの防御力より大きい場合、その差分プレイヤーHPが減る。
        {
            player.hp -= (attack - player.defence);
            printf("%d damege\n", (attack - player.defence));
            printf("player relive %d damage\n", player.defence);
        }
        else // プレイヤーの防御力が敵攻撃力より大きい場合、被ダメージ0
        {
            printf("Perfect Defence!\n");
        }
    }
    int is_Alive() // 生死判断
    {
        if (hp > 0)
        {
            return 0;
        }
        else
        {
            return -1;
        }
    }
    void check() // 敵情報の表示
    {
        printf("\nEnemy\n");
        if (hp <= 0)
        {
            printf("HP : 0\n");
        }
        else
        {
            printf("HP : %d\n", hp);
        }
    };

    void Action(Player &player, int choice)
    {
        const int maxHp = 40 + 10 * Level;
        // HPが半分未満のときだけ、25%の確率で回復する。
        if (hp < maxHp / 2 && rand() % 4 == 0)
        {
            Heal();
            return;
        }
        // HPにかかわらず、防御選択を反映する。
        if (choice == 2)
        {
            DefencedAttack(player);
        }
        else
        {
            Attack(player);
        }
    }
};

void Player::Levelup()
{
    // レベルを上げる前に、必要な経験値だけ消費する。
    experience -= Level * 5;
    Level++;
    hp += 10;
    attack += 2;
    defence += 5;
    speed += 5;
    printf("\nLevelUP!!, You become Lv%d!\n", Level);
}

void Player::gainexp(int exp)
{
    experience += exp;
    printf("\ngain exp %d!!\n", exp);
    while (experience >= Level * 5)
    {
        Levelup();
    }
}

// 1行を読み、指定範囲の整数だけを受け付ける。入力終了時は0を返す。
int readChoice(int minValue, int maxValue)
{
    std::string line;
    while (std::getline(std::cin, line))
    {
        std::istringstream input(line);
        int value;
        char extra;
        if ((input >> value) && !(input >> extra) &&
            value >= minValue && value <= maxValue)
        {
            return value;
        }
        printf("Please enter an integer from %d to %d.\n", minValue, maxValue);
    }
    return 0;
}

void playerAction(Player &player, Enemy &enemy, int choice)
{
    if (choice == 1)
    {
        player.Attack(enemy);
    }
    else if (choice == 2)
    {
        player.Defence();
    }
    else
    {
        player.Heal();
    }
}

// falseは入力終了、trueは戦闘終了を表す。
bool battle(Player &player, Enemy &enemy)
{
    printf("\nEnemy(Lv%d) comes!\n", enemy.Level);
    enemy.check();
    int turn = 1;

    while (player.is_Alive() == 0 && enemy.is_Alive() == 0)
    {
        printf("\nTurn %d\n", turn);
        printf("\nchoose action: 1. attack  2. defence  3. heal\n");
        // 両方の行動前に今ターンの選択を確定し、防御の持ち越しを防ぐ。
        const int choice = readChoice(1, 3);
        if (choice == 0)
        {
            return false;
        }

        if (player.speed >= enemy.speed)
        {
            playerAction(player, enemy, choice);
            if (enemy.is_Alive() == 0)
            {
                enemy.Action(player, choice);
            }
        }
        else
        {
            enemy.Action(player, choice);
            if (player.is_Alive() == 0)
            {
                playerAction(player, enemy, choice);
            }
        }
        enemy.check();
        player.check();
        turn++;
    }

    if (player.is_Alive() == -1)
    {
        printf("Be defeated, Game Over\n");
    }
    else
    {
        printf("Defeat Enemy!\n");
        player.gainexp(enemy.Level * 5);
    }
    return true;
}

bool startDungeon(Player &player, int difficulty) // ダンジョン生成関数
{
    int numEnemies, minLvl, maxLvl;
    if (difficulty == 1) // 難易度easy 1~5レベルの敵を2体生成
    {
        numEnemies = 2;
        minLvl = 1;
        maxLvl = 5;
    }
    else if (difficulty == 2) // 難易度normal 6~10レベルの敵を4体生成
    {
        numEnemies = 4;
        minLvl = 6;
        maxLvl = 10;
    }
    else if (difficulty == 3) // 難易度hard 11~15レベルの敵を6体生成
    {
        numEnemies = 6;
        minLvl = 11;
        maxLvl = 15;
    }
    else if (difficulty == 4) // 難易度Boss 20レベルのボスを生成
    {
        numEnemies = 1;
        minLvl = 20;
        maxLvl = 20;
    }
    else // 不正入力の場合動作終了
    {
        printf("wrong difficulty\n");
        return true;
    }
    printf("\n\nDungeon start!!\n");
    int i = 0;
    while (player.is_Alive() != -1 && i < numEnemies) // 敵の数分ループ
    {
        if (player.is_Alive() == 0) // プレイヤー生存時、敵レベルを決め、ステータスを生成し、バトル開始
        {
            int lv = rand() % (maxLvl - minLvl + 1) + minLvl;
            Enemy newEnemy(lv);
            printf("\nEnemy appears\n");
            if (!battle(player, newEnemy))
            {
                return false;
            }
        }
        i++; // 敵数を把握
    }
    if (i >= numEnemies && player.is_Alive() == 0) // 敵を倒した(=敵数が上限より大きい)かつプレイヤー生存時クリア表示
    {
        printf("\nComplete Dungeon !\n");
    }
    return true;
}

int main()
{
    srand(static_cast<unsigned int>(time(nullptr)));
    Player player(1);
    while (player.is_Alive() == 0)
    {
        printf("choose difficulty\n");
        printf("1: easy  2: normal  3: hard  4: Boss\n");
        const int difficulty = readChoice(1, 4);
        if (difficulty == 0 || !startDungeon(player, difficulty))
        {
            break;
        }
        if (player.is_Alive() == -1)
        {
            break;
        }
        printf("continue this game?\n");
        printf("continue: 1, finish: 2\n");
        if (readChoice(1, 2) != 1)
        {
            break;
        }
    }
    printf("finish\n");
    return 0;
}
