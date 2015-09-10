#pragma once

namespace AGE
{
	class WESerialization
	{
	private:
		static bool _forEditor;
	public:
		static inline bool SerializeForEditor()
		{
			return _forEditor;
		}
		static inline void SetSerializeForEditor(bool tof)
		{
			_forEditor = tof;
		}
	};
}