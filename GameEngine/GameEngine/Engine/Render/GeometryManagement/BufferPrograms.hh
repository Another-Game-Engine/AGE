#pragma once

# include <Render/Program.hh>
# include <Render/GeometryManagement/Buffer.hh>
# include <Render/GeometryManagement/Vertices.hh>
# include <Render/ProgramResources/AProgramResources.hh>
# include <Render/Key.hh>
# include <vector>

class BufferPrograms
{
public:
	BufferPrograms(std::vector<GLenum> &&types, std::vector<std::shared_ptr<Program>> const &programs);
	BufferPrograms(BufferPrograms &&move);

public:
	Key<Vertices> push_back(Vertices const &vertices);
	BufferPrograms &pop_bask();
	size_t size() const;
	BufferPrograms &bind();
	BufferPrograms &update();
	Buffer &operator[](size_t index);

private:
	std::vector<GLenum> _types;
	std::vector<Buffer> _buffers;
	std::vector<std::shared_ptr<Program>> _programs;
};