template<typename T>
class IVarChange
{
public:
	virtual void OnSliderChange(float sliderValue) = 0;
	void SetValue(T* value, T min, T max)
	{
		this->value = value;
		this->min = min;
		this->max = max;
	}
protected:
	T* value;
	T min;
	T max;
};

template<typename T>
class VarChange : public IVarChange<T>
{
private:
	VarChange() {}
	OnSliderChange(float sliderValue)
	{
		throw;
	}
};

template<> 
class VarChange<float> : public IVarChange<float>
{
	void OnSliderChange(float sliderValue)
	{
		*value = min + (max - min) * sliderValue;
	}
}

template<>
class VarChange<int> : public IVarChange<int>
{
	void OnSliderChange(float sliderValue)
	{
		*value = min + int((max - min) * sliderValue);
	}
}