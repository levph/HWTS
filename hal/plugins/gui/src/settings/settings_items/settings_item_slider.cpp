#include "gui/settings/settings_items/settings_item_slider.h"
#include "gui/settings/settings_manager.h"
#include "gui/settings/settings_widgets/settings_widget_slider.h"

#include <algorithm>

namespace hal
{
    SettingsItemSlider::SettingsItemSlider(const QString& label, const QString& tag, int defVal, const QString& cat, const QString& desc, bool isGlobal)
    {
        mMinimum = 0;
        mMaximum = 100;

        mLabel = label;
        mTag = tag;
        mValue = defVal;
        mDefaultValue = defVal;
        mCategory = cat;
        mDescription = desc;
        mIsGlobal = isGlobal;

        SettingsManager::instance()->registerSetting(this);
    }

    void SettingsItemSlider::setRange(int min, int max)
    {
        mMinimum = min;
        mMaximum = max;
    }

    QVariant SettingsItemSlider::value() const
    {
        return mValue;
    }

    QVariant SettingsItemSlider::defaultValue() const
    {
        return mDefaultValue;
    }

    void SettingsItemSlider::setDefaultValue(const QVariant& dv)
    {
        int newDefaultValue = dv.toInt();

        if(mDefaultValue == newDefaultValue)
            return;

        bool hasDefaultValue = (mValue == mDefaultValue);
        mDefaultValue = newDefaultValue;

        if (hasDefaultValue) setValue(mDefaultValue);
    }

    void SettingsItemSlider::setValue(const QVariant& v)
    {
        int newValue = v.toInt();

        if(mValue == newValue)
            return;

        mValue = newValue;
 
        Q_EMIT valueChanged();
        Q_EMIT intChanged(v.toInt());
    }

    SettingsWidget* SettingsItemSlider::editWidget(QWidget* parent)
    {
        //SettingsWidgetSlider* editWidget = new SettingsWidgetSlider(this, parent);
        //connect(editWidget, &SettingsWidgetSlider::valueChanged, this, &SettingsItemSlider::handleValueChanged);
        return new SettingsWidgetSlider(this, parent);
    }
}
