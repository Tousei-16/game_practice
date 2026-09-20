#include <iostream>
#include <stdio.h>
#include <ctime>

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

class Enemy; // PlayerクラスでEnemyクラスのメンバ変数が必要なため、先に宣言する。

class Player : public Character // サブクラスを定義する。
{
public:
    int defence;
    int experience;
    void Levelup(Enemy &enemy);
    void gainexp(int exp, Enemy &enemy);
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
        printf("\nPlayer recovers \n");
        hp += (50 + 10 * Level) / 2;
        if (hp > 50 + 10 * Level) // 回復で上限オーバーの場合、HPが上限値になる。
        {
            hp = 50 + 10 * Level;
        }
        int b;
        b = ((40 + 10 * Level) / 2);
        printf("recover %d Hp\n", b);
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
        printf("\nEnemy recovers\n");
        hp += (40 + 10 * Level) / 5;
        int a;
        a = ((40 + 10 * Level) / 5);
        printf("recover %d\n", a);
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
        if (hp >= ((40 + 10 * Level) / 2)) // 敵のHPが半分以上のとき、攻撃のみ
        {
            if (choice != 2) // プレイヤー非防御時
            {
                Attack(player);
            }
            else // プレイヤー防御時
            {
                DefencedAttack(player);
            }
        }
        else // 敵HPが半分以下の時
        {
            if (rand() % 4 != 0) // 75%で攻撃
            {
                Attack(player);
            }
            else // 25%で回復
            {
                Heal();
            }
        }
    }
};

void Player::Levelup(Enemy &enemy) // レベルアップの処理。
{
    Level++;
    hp += 10;
    attack += 2;
    defence += 5;
    speed += 5;
    if ((enemy.Level * 5) > Level * 5) // 経験値がレベルアップに必要な値より大きい場合、必要値との差分入る。
    {
        experience = (enemy.Level * 5 - Level * 5);
    }
    else // 経験値をレベルアップで使い果たした場合、経験値を0にする。
    {
        experience = 0;
    }
    printf("\nLevelUP!!, You  become Lv%d!\n", Level);
};

void Player::gainexp(int exp, Enemy &enemy) // 敵のレベルの５倍分経験値を得る。
{
    experience += exp;
    printf("\ngain exp %d!!\n", exp);
    if (experience >= Level * 5) // レベルアップ必要値を超えると、レベルアップ関数呼び出し
    {
        Levelup(enemy);
    }
};

void battle(Player &player, Enemy &enemy) // バトル時の処理関数
{
    printf("\nEnemy(Lv%d) comes!\n", enemy.Level);
    enemy.check();

    int choice = 1;
    int x = 1;

    while (player.is_Alive() == 0 && enemy.is_Alive() == 0 && 1 <= choice <= 3) // プレイヤー・敵両方生存時
    {
        if (player.speed >= enemy.speed) // プレイヤーの素早さが敵の値よりも大きい場合
        {

            printf("\nTurn %d\n", x);                                     // ターン数表示
            printf("\n choose action: 1. attack  2. defence  3. heal\n"); // プレイヤー行動選択
            scanf("%d", &choice);                                         // キーボード入力を待つ
            if (choice == 1)                                              // 攻撃
            {
                player.Attack(enemy);
            }
            else if (choice == 2) // 防御
            {
                player.Defence();
            }
            else if (choice == 3) // 回復
            {
                player.Heal();
            }
            else // 不正入力時
            {
                printf("Your choice is wrong\n");
                printf("please rechoose action\n");
                printf("\n choose action: 1. attack  2. defence  3. heal\n"); // 再入力
                scanf("%d", &choice);
                if (choice == 1)
                {
                    player.Attack(enemy);
                }
                else if (choice == 2)
                {
                    player.Defence();
                }
                else if (choice == 3)
                {
                    player.Heal();
                }
                else // 不正入力時
                {
                    player.hp = 0;
                    break;
                }
            }
            enemy.check();
            if (enemy.is_Alive() == 0) // 敵生存の場合、敵の行動
            {
                enemy.Action(player, choice);
            }
            player.check();
            x++; // ターン数追加
        }
        else
        {
            printf("\nTurn %d\n", x);
            if (enemy.is_Alive() == 0) // 敵生存の場合、敵の行動
            {
                enemy.Action(player, choice);
            }
            player.check();
            if (player.is_Alive() == 0) // プレイヤー生存の場合、行動選択
            {
                printf("\n choose action: 1. attack  2. defence  3. heal\n");
                scanf("%d", &choice);
                if (choice == 1)
                {
                    player.Attack(enemy);
                }
                else if (choice == 2)
                {
                    player.Defence();
                }
                else if (choice == 3)
                {
                    player.Heal();
                }
                else // 不正入力時
                {
                    printf("Your choice is wrong\n");
                    printf("please rechoose action\n");
                    printf("\n choose action: 1. attack  2. defence  3. heal\n"); // 再入力
                    scanf("%d", &choice);
                    if (choice == 1)
                    {
                        player.Attack(enemy);
                    }
                    else if (choice == 2)
                    {
                        player.Defence();
                    }
                    else if (choice == 3)
                    {
                        player.Heal();
                    }
                    else // 二回目の不正入力の場合ゲームオーバー
                    {
                        player.hp = 0;
                        break;
                    }
                }
            }
            enemy.check();
            x++; // ターン数追加
        }
    }
    if (player.is_Alive() == -1) // プレイヤー死亡時、ゲームオーバー
    {
        printf("Be defeated , Game Over\n");
    }
    else // 敵を倒したとき、経験値を得る関数を呼び出す。
    {
        printf("Defeat Enemy!\n");
        player.gainexp(enemy.Level * 5, enemy);
        x = 1;
    }
}

void startDungeon(Player &player, int difficulty) // ダンジョン生成関数
{
    int numEnemies, minLvl, maxLvl;
    if (difficulty == 1) // 難易度easy 1~5レベルの敵を2体生成
    {
        numEnemies = 2;
        minLvl = 1;
        maxLvl = 5;
    }
    else if (difficulty == 2) // 難易度normal 5~10レベルの敵を4体生成
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
        return;
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
            battle(player, newEnemy);
        }
        i++; // 敵数を把握
    }
    if (i >= numEnemies && player.is_Alive() == 0) // 敵を倒した(=敵数が上限より大きい)かつプレイヤー生存時クリア表示
    {
        printf("\nComplete Dungeon !\n");
    }
}

int main()
{
    srand(time(0));   // 乱数の種を設定する。
    Player player(1); // レベル１でステータス生成
    int keizoku;      // ゲーム続行の判別変数
    keizoku = 1;
    while (keizoku != 2) // ループ中はゲーム続行
    {
        printf("choose difficulty\n");
        printf("1: easy  2: normal  3: hard  4:Boss\n");
        int difficulty;
        scanf("%d", &difficulty);
        startDungeon(player, difficulty);
        if (player.is_Alive() == 0) // ダンジョンクリア時
        {
            printf("continue this game？\n");
            printf("continue : 1 , finish : 2\n");
            scanf("%d", &keizoku); // ゲーム続行の入力
        }
        else // ゲームオーバー時
        {
            keizoku = 2; // ゲーム終了
        }
    }
    printf("finish\n");
    return 0;
}