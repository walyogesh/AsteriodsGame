#include "Game.h"
#include "System.h"
#include "OrthoCamera.h"
#include "Background.h"
#include "Ship.h"
#include "Asteroid.h"
#include "Explosion.h"
#include "Keyboard.h"
#include "Random.h"
#include "Maths.h"
#include "Bullet.h"
#include "Collision.h"
#include <algorithm>
#include <typeinfo>
#include <string>

Game::Game() :
	camera_(0),
	background_(0),
    player_(0),
	collision_(0),
	playerLife_(0),	
	score_(0)
{
	camera_ = new OrthoCamera();
	camera_->SetPosition(XMFLOAT3(0.0f, 0.0f, 0.0f));
	camera_->SetFrustum(800.0f, 600.0f, -100.0f, 100.0f);
	background_ = new Background(800.0f, 600.0f);
	collision_ = new Collision();
	SetSessionName("Yogesh-");
}

Game::~Game()
{
	delete camera_;
	delete background_;
	delete player_;
	//DeleteBullet();
	DeleteAllAsteroids();
	DeleteAllExplosions();
	delete collision_;
}

void Game::Update(System *system)
{
	UpdatePlayer(system);
	UpdateAsteroids(system);
	UpdateBullet(system);
	UpdateCollisions();
	DeleteOutOfWindowBullets();
}

void Game::RenderBackgroundOnly(Graphics *graphics)
{
	camera_->SetAsView(graphics);
	background_->Render(graphics);
}

void Game::RenderEverything(Graphics *graphics)
{
	camera_->SetAsView(graphics);

	background_->Render(graphics);

	if (player_)
	{
		player_->Render(graphics);
	}

	for (AsteroidList::const_iterator asteroidIt = asteroids_.begin(),
		end = asteroids_.end();
		asteroidIt != end;
		++asteroidIt)
	{
		(*asteroidIt)->Render(graphics);
	}

	if (!bullet_.empty())
	{
		for (auto i : bullet_)
		{
			i->Render(graphics);
		}
	}

	for (ExplosionList::const_iterator explosionIt = explosions_.begin(),
		end = explosions_.end();
		explosionIt != end;
		++explosionIt)
	{
		(*explosionIt)->Render(graphics);
	}
}

void Game::InitialiseLevel(int numAsteroids)
{
	DeleteAllAsteroids();
	DeleteAllExplosions();
	DeleteAllBullets();
   	SpawnPlayer();
	SpawnAsteroids(numAsteroids);
}

bool Game::IsLevelComplete() const
{
	return (asteroids_.empty() && explosions_.empty());
}

bool Game::IsGameOver() const
{
	return (playerLife_ == 0 && player_ == 0 && explosions_.empty());
}

bool Game::DoCollision(GameEntity* a, GameEntity* b)
{
	Ship* player = static_cast<Ship*>(typeid(*a) == typeid(Ship) ? a : (typeid(*b) == typeid(Ship) ? b : 0));
	Asteroid* asteroid = static_cast<Asteroid*>(typeid(Asteroid) == typeid(*a) ? a : (typeid(Asteroid) == typeid(*b) ? b : 0));
	Bullet* bullet = static_cast<Bullet*>(typeid(Bullet) == typeid(*a) ? a : (typeid(Bullet) == typeid(*b) ? b : 0));
	return (TryPlayerAndAstroidCollision(player, asteroid) || TryBulletAndAstroidCollision(bullet, asteroid));
}

bool Game::TryPlayerAndAstroidCollision(Ship *player, Asteroid *asteroid)
{
  	if (player && asteroid)
	{
		AsteroidHit(asteroid);
		DeletePlayer();
		if (playerLife_)
		{
			SpawnPlayer();
		}
		ReducePlayerLife();
		return true;
	}
	return false;
}

bool Game::TryBulletAndAstroidCollision(Bullet* bullet, Asteroid* asteroid)
{
	if (bullet && asteroid)
	{
		AsteroidHit(asteroid);
		DeleteBullet(bullet);
			AddScore();
		return true;
	}
	return false;
}

void Game::SpawnPlayer()
{
	DeletePlayer();
	player_ = new Ship();
	player_->EnableCollisions(collision_, 10.0f);
}

void Game::DeletePlayer()
{
	delete player_;
	player_ = 0;
}

void Game::UpdatePlayer(System *system)
{
	if (player_ == 0)
		return;

	Keyboard *keyboard = system->GetKeyboard();

	float acceleration = 0.0f;
	if (keyboard->IsKeyHeld(VK_UP) || keyboard->IsKeyHeld('W'))
	{
		acceleration = 1.0f;
	}
	else if (keyboard->IsKeyHeld(VK_DOWN) || keyboard->IsKeyHeld('S'))
	{
		acceleration = -1.0f;
	}

	float rotation = 0.0f;
	if (keyboard->IsKeyHeld(VK_RIGHT) || keyboard->IsKeyHeld('D'))
	{
		rotation = -1.0f;
	}
	else if (keyboard->IsKeyHeld(VK_LEFT) || keyboard->IsKeyHeld('A'))
	{
		rotation = 1.0f;
	}

	player_->SetControlInput(acceleration, rotation);
	player_->Update(system);
	WrapEntity(player_);

	if (keyboard->IsKeyPressed(VK_SPACE))
	{
		XMVECTOR playerForward = player_->GetForwardVector();
		XMVECTOR bulletPosition = player_->GetPosition() + playerForward * 10.0f;
		SpawnBullet(bulletPosition, playerForward);
	}
}

void Game::UpdateAsteroids(System *system)
{
	for (AsteroidList::const_iterator asteroidIt = asteroids_.begin(),
		end = asteroids_.end();
		asteroidIt != end;
	++asteroidIt)
	{
		(*asteroidIt)->Update(system);
		WrapEntity(*asteroidIt);
	}
}

void Game::UpdateBullet(System *system)
{
	if (!bullet_.empty())
	{
		for (auto i : bullet_)
		{
			i->Update(system);
			WrapEntity(i);
		}
	}
}

void Game::WrapEntity(GameEntity *entity) const
{
	XMFLOAT3 entityPosition;
	XMStoreFloat3(&entityPosition, entity->GetPosition());
	entityPosition.x = Maths::WrapModulo(entityPosition.x, -400.0f, 400.0f);
	entityPosition.y = Maths::WrapModulo(entityPosition.y, -300.0f, 300.0f);
	entity->SetPosition(XMLoadFloat3(&entityPosition));
}

void Game::DeleteAllAsteroids()
{
	for (AsteroidList::const_iterator asteroidIt = asteroids_.begin(),
		end = asteroids_.end();
		asteroidIt != end;
		++asteroidIt)
	{
		delete (*asteroidIt);
	}

	asteroids_.clear();
}

void Game::DeleteAllExplosions()
{
	for (ExplosionList::const_iterator explosionIt = explosions_.begin(),
		end = explosions_.end();
		explosionIt != end;
	++explosionIt)
	{
		delete (*explosionIt);
	}

	explosions_.clear();
}

void Game::SpawnBullet(XMVECTOR position, XMVECTOR direction)
{
	auto currentBullet = new Bullet(position, direction);
	bullet_.push_back( currentBullet );
	currentBullet->EnableCollisions(collision_, 3.0f);
}

void Game::DeleteBullet(Bullet* bulletToDelete)
{
	if (bulletToDelete)
	{
		bullet_.remove(bulletToDelete);
	}
}

void Game::DeleteOutOfWindowBullets()
{
	bullet_.remove_if( [](Bullet* bullet) {
 		return bullet->IsOutOfWindow(); });
}

void Game::SpawnAsteroids(int numAsteroids)
{
	float halfWidth = 800.0f * 0.5f;
	float halfHeight = 600.0f * 0.5f;
	for (int i = 0; i < numAsteroids; i++)
	{
		float x = Random::GetFloat(-halfWidth, halfWidth);
		float y = Random::GetFloat(-halfHeight, halfHeight);
		XMVECTOR position = XMVectorSet(x, y, 0.0f, 0.0f);
		SpawnAsteroidAt(position, 3);
	}
}

void Game::SpawnAsteroidAt(XMVECTOR position, int size)
{
	const float MAX_ASTEROID_SPEED = 1.0f;

	float angle = Random::GetFloat(Maths::TWO_PI);
	XMMATRIX randomRotation = XMMatrixRotationZ(angle);
	XMVECTOR velocity = XMVectorSet(0.0f, Random::GetFloat(MAX_ASTEROID_SPEED), 0.0f, 0.0f);
	velocity = XMVector3TransformNormal(velocity, randomRotation);

	Asteroid *asteroid = new Asteroid(position, velocity, size);
	asteroid->EnableCollisions(collision_, size * 5.0f);
	asteroids_.push_back(asteroid);
}

bool Game::IsAsteroid(GameEntity *entity) const
{
	return (std::find(asteroids_.begin(),
		asteroids_.end(), entity) != asteroids_.end()); 
}

void Game::AsteroidHit(Asteroid *asteroid)
{
	int oldSize = asteroid->GetSize();
	if (oldSize > 1)
	{
		int smallerSize = oldSize -1;
		XMVECTOR position = asteroid->GetPosition();
		SpawnAsteroidAt(position, smallerSize);
		SpawnAsteroidAt(position, smallerSize);
	}
	DeleteAsteroid(asteroid);
}

void Game::DeleteAsteroid(Asteroid *asteroid)
{
	asteroids_.remove(asteroid);
	delete asteroid;
}

void Game::UpdateCollisions()
{
	collision_->DoCollisions(this);
}
