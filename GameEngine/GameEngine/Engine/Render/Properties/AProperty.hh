#pragma once

# include <Render/Properties/IProperty.hh>
# include <Render/Program.hh>
# include <utility>

namespace AGE
{
	class AProperty : public IProperty
	{
	protected:
		AProperty(std::string &&name);
		AProperty(AProperty &&move);

	public:
		virtual std::string const &name() const override final;
		virtual std::shared_ptr<IProgramResources> get_resource(std::shared_ptr<Program> const &program) override final;
	private:

#ifdef AGE_DEBUG
		std::size_t _shaderVersion;
#endif
		std::string _name;
		std::vector<std::pair<std::shared_ptr<Program>, std::shared_ptr<IProgramResources>>> _registered_resources;
	};
}
