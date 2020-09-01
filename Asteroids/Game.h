#ifndef GAME_H_INCLUDED
#define GAME_H_INCLUDED

#define MAXLIFES                 5
#include <DirectXMath.h>
#include <list>
#include "string"

using namespace DirectX;

class OrthoCamera;
class Background;
class Ship;
class Bullet;
class Asteroid;
class Explosion;
class Collision;
class System;
class Graphics;
class GameEntity;

class Game
{
public:
	Game();
	~Game();

	void Update(System *system);
	void RenderBackgroundOnly(Graphics *graphics);
	void RenderEverything(Graphics *graphics);

	void InitialiseLevel(int numAsteroids);
	bool IsLevelComplete() const;
	bool IsGameOver() const;
	bool DoCollision(GameEntity *a, GameEntity *b);
    inline void AddPlayerLife(unsigned int life = 1)    { playerLife_ = playerLife_ < MAXLIFES ? playerLife_ + life : playerLife_; }
	inline void ReducePlayerLife(unsigned int life = 1) { playerLife_ = playerLife_ ? playerLife_ - life : 0; }
	inline unsigned int GetPlayerLife() const { return playerLife_; }
	inline int GetScore() const {return score_;}
	inline std::string GetSessionName() const { return sessionName_; }
	inline void SetSessionName(std::string val) { sessionName_ = val; }
	inline void DeleteAllBullets() { bullet_.clear(); }
	inline void ClearScore(){ score_ = 0; }
private:
	Game(const Game &);
	void operator=(const Game &);

	bool TryBulletAndAstroidCollision(Bullet* bullet, Asteroid* asteroid);
	bool TryPlayerAndAstroidCollision(Ship* bullet, Asteroid* asteroid);
	typedef std::list<Asteroid *> AsteroidList;
	typedef std::list<Explosion *> ExplosionList;

	void SpawnPlayer();
	void DeletePlayer();

	void UpdatePlayer(System *system);
	void UpdateAsteroids(System *system);
	void UpdateBullet(System *system);
	void WrapEntity(GameEntity *entity) const;

	void DeleteAllAsteroids();
	void DeleteAllExplosions();

	void SpawnBullet(XMVECTOR position, XMVECTOR direction);
	void DeleteBullet(Bullet*);
	void DeleteOutOfWindowBullets();
	void DeleteAllBullet();

	void SpawnAsteroids(int numAsteroids);
	void SpawnAsteroidAt(XMVECTOR position, int size);
	bool IsAsteroid(GameEntity *entity) const;
	void AsteroidHit(Asteroid *asteroid);
	void DeleteAsteroid(Asteroid *asteroid);

	void UpdateCollisions();

	inline void AddScore() { score_ += 10; }

	OrthoCamera *camera_;

	Background *background_;
	Ship *player_;
	std::list<Bullet*> bullet_;
	AsteroidList asteroids_;
	ExplosionList explosions_;

	Collision *collision_;
	unsigned int playerLife_;
	unsigned int score_;
	std::string sessionName_;
};

#endif // GAME_H_INCLUDED
