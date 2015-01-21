#pragma once

# include <Render/Properties/AProperty.hh>

class Texture2D;

class MapColor : public AProperty
{
public:
	MapColor(std::string const &add_name);
	MapColor(MapColor &&move);

public:
	virtual IProperty & update(std::shared_ptr<Program> const &p) override;

public:
	std::shared_ptr<Texture2D> const &get() const;
	MapColor &set(std::shared_ptr<Texture2D> const &map);
	
private:
	std::shared_ptr<Texture2D> _texture;
};