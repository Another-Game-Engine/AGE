#pragma once

namespace AGE
{
	class WESerialization
	{
	private:
		static bool _forEditor;
	public:
		static bool SerializeForEditor()
		{
			return _forEditor;
		}
		static void SetSerializeForEditor(bool tof)
		{
			_forEditor = tof;
		}
	};
}