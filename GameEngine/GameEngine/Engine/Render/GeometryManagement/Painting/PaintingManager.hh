#pragma once

# include <Utils/OpenGL.hh>
# include <vector>
# include <memory>
# include <Render/Key.hh>

class Painter;

class PaintingManager
{
public:
	PaintingManager();
	PaintingManager(PaintingManager &&move);
	PaintingManager(PaintingManager const &copy) = delete;
	PaintingManager &operator=(PaintingManager const &p) = delete;

public:
	Key<Painter> add_painter(std::vector<GLenum> &&types);
	std::shared_ptr<Painter> const &get_painter(Key<Painter> const &key) const;
	Key<Painter> get_painter(std::vector<GLenum> const &types) const;
	bool has_painter(std::vector<GLenum> const types) const;
	void print() const;

private:
	std::vector<std::shared_ptr<Painter>> _painters;
};