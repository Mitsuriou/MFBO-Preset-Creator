#include "DataLists.h"
#include "Utils.h"

QStringList DataLists::GetBodyNamesList()
{
  return QStringList{QString("CBBE 3BA (3BBB) - by Acro"),
                     QString("BHUNP (UUNP Next Generation) SSE - by Bakafactory and Haeun"),
                     QString("CBBE SMP (3BBB) - by Ousnius"),
                     QString("COCO Body CBBE - by COCO"),
                     QString("COCO Body UUNP - by COCO"),
                     QString("Mimir Ebonic Body V.2 - by Medley")};
}

QStringList DataLists::GetBodyVersionsList(const BodyName& aBodyName)
{
  switch (aBodyName)
  {
    case BodyName::CBBE_3BA_3BBB:
    {
      return QStringList({QString("1.50"), QString("1.51 to 1.55"), QString("2.02 to 2.04"), QString("2.06"), QString("2.11 and 2.12"), QString("2.13 and 2.14")});
    }
    case BodyName::BHUNP_UUNP_NEXT_GENERATION:
    {
      return QStringList({QString("2.20"), QString("2.25"), QString("2.30"), QString("2.31"), QString("2.35 to 2.41")});
    }
    case BodyName::CBBE_SMP_3BBB:
    {
      return QStringList(QString("1.2.0"));
    }
    case BodyName::COCO_BODY_CBBE:
    {
      return QStringList({QString("V4"), QString("V6")});
    }
    case BodyName::COCO_BODY_UUNP:
    {
      return QStringList({QString("V3"), QString("V4"), QString("V6")});
    }
    case BodyName::MIMIR_EBONIC_BODY:
    {
      return QStringList({QString("1.2"), QString("1.2 foot fix")});
    }
    case BodyName::_INVALID_VALUE:
    default:
    {
      return QStringList(QString());
    }
  }
}

QStringList DataLists::GetBodyVariantsList(const BodyName& aBodyName, const int aRelativeVersion)
{
  switch (aBodyName)
  {
    case BodyName::CBBE_3BA_3BBB:
    {
      // All the version numbers propose the variant below:
      return QStringList{QString("CBBE 3BBB Body Amazing")};
    }
    case BodyName::BHUNP_UUNP_NEXT_GENERATION:
    {
      // All the version numbers propose the variants below:
      QStringList lVariantsList{QString("BHUNP 3BBB"),
                                QString("BHUNP 3BBB Advanced"),
                                QString("BHUNP 3BBB Advanced Ver 2"),
                                QString("BHUNP BBP"),
                                QString("BHUNP BBP Advanced"),
                                QString("BHUNP TBBP"),
                                QString("BHUNP TBBP Advanced")};

      // Only for 2.25 and 2.30
      if (aRelativeVersion == 1 || aRelativeVersion == 2)
      {
        lVariantsList.append(QString("BHUNP 3BBB Advanced Ver 2 (Nevernude)"));
      }

      return lVariantsList;
    }
    case BodyName::CBBE_SMP_3BBB:
    {
      // All the version numbers propose the variant below:
      return QStringList{QString("CBBE Body SMP (3BBB)")};
    }
    case BodyName::COCO_BODY_CBBE:
    {
      // All the version numbers propose the variant below:
      return QStringList{QString("COCO Body CBBE")};
    }
    case BodyName::COCO_BODY_UUNP:
    {
      // All the version numbers propose the variant below:
      return QStringList{QString("COCO Body UUNP")};
    }
    case BodyName::MIMIR_EBONIC_BODY:
    {
      // All the version numbers propose the variant below:
      return QStringList{QString("Mimir Ebonic Body")};
    }
    case BodyName::_INVALID_VALUE:
    default:
    {
      return QStringList(QString());
    }
  }
}

QString DataLists::GetQRCRessourceName(const BodyVariant& aBodyVariant)
{
  switch (aBodyVariant)
  {
    case BodyVariant::CBBE_3BA_3BBB:
      return QString("cbbe 3bbb 3ba");
    case BodyVariant::BHUNP_3BBB:
      return QString("bhunp 3bbb");
    case BodyVariant::BHUNP_3BBB_ADVANCED:
      return QString("bhunp 3bbb advanced");
    case BodyVariant::BHUNP_3BBB_ADVANCED_VER_2:
      return QString("bhunp 3bbb advanced ver 2");
    case BodyVariant::BHUNP_BBP:
      return QString("bhunp bbp");
    case BodyVariant::BHUNP_BBP_ADVANCED:
      return QString("bhunp bbp advanced");
    case BodyVariant::BHUNP_TBBP:
      return QString("bhunp tbbp");
    case BodyVariant::BHUNP_TBBP_ADVANCED:
      return QString("bhunp tbbp advanced");
    case BodyVariant::BHUNP_3BBB_ADVANCED_VER_2_NEVERNUDE:
      return QString("bhunp 3bbb advanced ver 2 nevernude");
    case BodyVariant::CBBE_SMP_3BBB:
      return QString("cbbe smp 3bbb");
    case BodyVariant::COCO_BODY_CBBE:
      return QString("coco body cbbe");
    case BodyVariant::COCO_BODY_UUNP:
      return QString("coco body uunp");
    case BodyVariant::MIMIR_EBONIC_BODY:
      return QString("mimir ebonic body");
    case BodyVariant::_INVALID_VALUE:
    default:
      return QString();
  }
}

QStringList DataLists::GetFeetNamesList(const BodyVariant& aBodyVariant)
{
  switch (aBodyVariant)
  {
    case BodyVariant::CBBE_3BA_3BBB:
    {
      return QStringList{QString("CBBE 3BBB Feet - by Acro"),
                         QString("More Sliders for Feet - by Balveric")};
    }
    case BodyVariant::BHUNP_3BBB:
    case BodyVariant::BHUNP_3BBB_ADVANCED:
    case BodyVariant::BHUNP_3BBB_ADVANCED_VER_2:
    case BodyVariant::BHUNP_BBP:
    case BodyVariant::BHUNP_BBP_ADVANCED:
    case BodyVariant::BHUNP_TBBP:
    case BodyVariant::BHUNP_TBBP_ADVANCED:
    case BodyVariant::BHUNP_3BBB_ADVANCED_VER_2_NEVERNUDE:
    {
      return QStringList{QString("BHUNP 3BBB Advanced Feet - by Bakafactory and Haeun"),
                         QString("More Sliders for Feet - by Balveric"),
                         QString("HG Feet and Toes UNP - by SunJeong"),
                         QString("K Feet - by Khrysamere")};
    }
    case BodyVariant::CBBE_SMP_3BBB:
    {
      return QStringList{QString("CBBE Default Feet - by Ousnius"),
                         QString("More Sliders for Feet - by Balveric")};
    }
    case BodyVariant::COCO_BODY_CBBE:
    {
      return QStringList{QString("COCO Feet - by COCO"),
                         QString("More Sliders for Feet - by Balveric")};
    }
    case BodyVariant::COCO_BODY_UUNP:
    {
      return QStringList{QString("COCO Feet - by COCO"),
                         QString("More Sliders for Feet - by Balveric"),
                         QString("HG Feet and Toes UNP - by SunJeong"),
                         QString("K Feet - by Khrysamere")};
    }
    case BodyVariant::MIMIR_EBONIC_BODY:
    {
      return QStringList{QString("CBBE 3BBB Feet - by Medley"),
                         QString("More Sliders for Feet - by Balveric")};
    }
    case BodyVariant::_INVALID_VALUE:
    default:
    {
      return QStringList(QString());
    }
  }
}

QStringList DataLists::GetFeetVersionsList(const FeetName& aFeetName, const bool aIsCBBEBody)
{
  switch (aFeetName)
  {
    case FeetName::CBBE:
      return QStringList{QString("1.6.1")};
    case FeetName::BHUNP:
      return QStringList{QString("2.20"), QString("2.25"), QString("2.30"), QString("2.31"), QString("2.35 to 2.41")};
    case FeetName::CBBE_3BA_3BBB:
      return QStringList{QString("1.50"), QString("1.51 to 1.55"), QString("2.02 to 2.04"), QString("2.06"), QString("2.11 and 2.12"), QString("2.13 and 2.14")};
    case FeetName::COCO_BODY:
      if (aIsCBBEBody)
        return QStringList{QString("V4"), QString("V6")};

      return QStringList{QString("V3"), QString("V4"), QString("V6")};
    case FeetName::MIMIR_EBONIC_BODY:
      return QStringList{QString("1.2"), QString("1.2 foot fix")};
    case FeetName::MORE_SLIDERS_FOR_FEET:
      if (aIsCBBEBody)
        return QStringList{QString("1.0")};
      return QStringList{QString("1.1")};
    case FeetName::HG_FEET:
      return QStringList{QString("1")};
    case FeetName::KHRYSAMERE_HG_FEET:
      return QStringList{QString("1.0"), QString("1.3.3"), QString("1.4"), QString("1.5"), QString("1.6")};
    case FeetName::_INVALID_VALUE:
    default:
      return QStringList(QString());
  }
}

QStringList DataLists::GetFeetVariantsList(const FeetName& aFeetName)
{
  switch (aFeetName)
  {
    case FeetName::CBBE:
      return QStringList{QString("CBBE Default Feet")};
    case FeetName::BHUNP:
      return QStringList{QString("BHUNP 3BBB Advanced Feet")};
    case FeetName::CBBE_3BA_3BBB:
      return QStringList{QString("CBBE 3BBB Feet")};
    case FeetName::COCO_BODY:
      return QStringList{QString("COCO Feet")};
    case FeetName::MIMIR_EBONIC_BODY:
      return QStringList{QString("CBBE 3BBB Feet")};
    case FeetName::MORE_SLIDERS_FOR_FEET:
      return QStringList{QString("More Sliders for Feet - Normal"),
                         QString("More Sliders for Feet - High Heels")};
    case FeetName::HG_FEET:
      return QStringList{QString("HGFeet UUNP")};
    case FeetName::KHRYSAMERE_HG_FEET:
      return QStringList{QString("K Feet")};
    case FeetName::_INVALID_VALUE:
    default:
      return QStringList(QString());
  }
}

QString DataLists::GetQRCRessourceName(const BodyVariant& aBodyVariant, const FeetNameVersion& aFeetNameVersion)
{
  auto lFeetName{DataLists::GetName(aFeetNameVersion)};
  switch (lFeetName)
  {
    case FeetName::CBBE:
      return QString("cbbe");
    case FeetName::BHUNP:
      return QString("bhunp");
    case FeetName::CBBE_3BA_3BBB:
      return QString("cbbe 3bbb 3ba");
    case FeetName::COCO_BODY:
      if (Utils::IsCBBEBasedBody(aBodyVariant))
        return QString("coco body cbbe");
      return QString("coco body uunp");
    case FeetName::MIMIR_EBONIC_BODY:
      return QString("mimir ebonic body");
    case FeetName::MORE_SLIDERS_FOR_FEET:
      if (aFeetNameVersion == FeetNameVersion::MORE_SLIDERS_FOR_FEET_NORMAL_CBBE_1_0)
        return QString("msf/normal/cbbe");
      else if (aFeetNameVersion == FeetNameVersion::MORE_SLIDERS_FOR_FEET_NORMAL_BHUNP_1_1)
        return QString("msf/normal/bhunp");
      else if (aFeetNameVersion == FeetNameVersion::MORE_SLIDERS_FOR_FEET_HIGH_HEELS_CBBE_1_0)
        return QString("msf/high heels/cbbe");
      else if (aFeetNameVersion == FeetNameVersion::MORE_SLIDERS_FOR_FEET_HIGH_HEELS_BHUNP_1_1)
        return QString("msf/high heels/bhunp");
      return QString(); // invalid case
    case FeetName::HG_FEET:
      if (Utils::IsCBBEBasedBody(aBodyVariant))
      {
        return QString(); // invalid case
      }
      return QString("hg feet/bhunp");
    case FeetName::KHRYSAMERE_HG_FEET:
      return QString("khrysamere hg feet");
    case FeetName::_INVALID_VALUE:
    default:
      return QString();
  }
}

BodyName DataLists::GetName(const BodyVariant& aBodyVariant)
{
  // Return the parent body name of a given variant
  switch (aBodyVariant)
  {
    case BodyVariant::CBBE_3BA_3BBB:
      return BodyName::CBBE_3BA_3BBB;
    case BodyVariant::BHUNP_3BBB:
    case BodyVariant::BHUNP_3BBB_ADVANCED:
    case BodyVariant::BHUNP_3BBB_ADVANCED_VER_2:
    case BodyVariant::BHUNP_BBP:
    case BodyVariant::BHUNP_BBP_ADVANCED:
    case BodyVariant::BHUNP_TBBP:
    case BodyVariant::BHUNP_TBBP_ADVANCED:
    case BodyVariant::BHUNP_3BBB_ADVANCED_VER_2_NEVERNUDE:
      return BodyName::BHUNP_UUNP_NEXT_GENERATION;
    case BodyVariant::CBBE_SMP_3BBB:
      return BodyName::CBBE_SMP_3BBB;
    case BodyVariant::COCO_BODY_CBBE:
      return BodyName::COCO_BODY_CBBE;
    case BodyVariant::COCO_BODY_UUNP:
      return BodyName::COCO_BODY_UUNP;
    case BodyVariant::MIMIR_EBONIC_BODY:
      return BodyName::MIMIR_EBONIC_BODY;
    case BodyVariant::_INVALID_VALUE:
    default:
      return BodyName::_INVALID_VALUE;
  }
}

FeetName DataLists::GetName(const FeetVariant& aFeetVariant)
{
  switch (aFeetVariant)
  {
    case FeetVariant::CBBE:
      return FeetName::CBBE;
    case FeetVariant::BHUNP:
      return FeetName::BHUNP;
    case FeetVariant::CBBE_3BA_3BBB:
      return FeetName::CBBE_3BA_3BBB;
    case FeetVariant::COCO_BODY_CBBE:
    case FeetVariant::COCO_BODY_UUNP:
      return FeetName::COCO_BODY;
    case FeetVariant::MIMIR_EBONIC_BODY:
      return FeetName::MIMIR_EBONIC_BODY;
    case FeetVariant::MORE_SLIDERS_FOR_FEET_NORMAL_CBBE:
    case FeetVariant::MORE_SLIDERS_FOR_FEET_NORMAL_BHUNP:
    case FeetVariant::MORE_SLIDERS_FOR_FEET_HIGH_HEELS_CBBE:
    case FeetVariant::MORE_SLIDERS_FOR_FEET_HIGH_HEELS_BHUNP:
      return FeetName::MORE_SLIDERS_FOR_FEET;
    case FeetVariant::HG_FEET:
      return FeetName::HG_FEET;
    case FeetVariant::KHRYSAMERE_HG_FEET:
      return FeetName::KHRYSAMERE_HG_FEET;
    case FeetVariant::_INVALID_VALUE:
    default:
      return FeetName::_INVALID_VALUE;
  }
}

BodyName DataLists::GetName(const BodyNameVersion& aBodyNameVersion)
{
  return DataLists::GetName(DataLists::GetVariant(aBodyNameVersion));
}

FeetName DataLists::GetName(const FeetNameVersion& aFeetNameVersion)
{
  return DataLists::GetName(DataLists::GetVariant(aFeetNameVersion));
}

FeetName DataLists::GetName(const BodyVariant& aBodyVariant, const int aRelativeFeetName)
{
  switch (aBodyVariant)
  {
    case BodyVariant::CBBE_3BA_3BBB:
    {
      switch (aRelativeFeetName)
      {
        case 0:
          return FeetName::CBBE_3BA_3BBB;
        case 1:
          return FeetName::MORE_SLIDERS_FOR_FEET;
        default:
          return FeetName::_INVALID_VALUE;
      }
    }
    case BodyVariant::BHUNP_3BBB:
    case BodyVariant::BHUNP_3BBB_ADVANCED:
    case BodyVariant::BHUNP_3BBB_ADVANCED_VER_2:
    case BodyVariant::BHUNP_BBP:
    case BodyVariant::BHUNP_BBP_ADVANCED:
    case BodyVariant::BHUNP_TBBP:
    case BodyVariant::BHUNP_TBBP_ADVANCED:
    case BodyVariant::BHUNP_3BBB_ADVANCED_VER_2_NEVERNUDE:
    {
      switch (aRelativeFeetName)
      {
        case 0:
          return FeetName::BHUNP;
        case 1:
          return FeetName::MORE_SLIDERS_FOR_FEET;
        case 2:
          return FeetName::HG_FEET;
        case 3:
          return FeetName::KHRYSAMERE_HG_FEET;
        default:
          return FeetName::_INVALID_VALUE;
      }
    }
    case BodyVariant::CBBE_SMP_3BBB:
    {
      switch (aRelativeFeetName)
      {
        case 0:
          return FeetName::CBBE;
        case 1:
          return FeetName::MORE_SLIDERS_FOR_FEET;
        default:
          return FeetName::_INVALID_VALUE;
      }
    }
    case BodyVariant::COCO_BODY_CBBE:
    {
      switch (aRelativeFeetName)
      {
        case 0:
          return FeetName::COCO_BODY;
        case 1:
          return FeetName::MORE_SLIDERS_FOR_FEET;
        default:
          return FeetName::_INVALID_VALUE;
      }
    }
    case BodyVariant::COCO_BODY_UUNP:
    {
      switch (aRelativeFeetName)
      {
        case 0:
          return FeetName::COCO_BODY;
        case 1:
          return FeetName::MORE_SLIDERS_FOR_FEET;
        case 2:
          return FeetName::HG_FEET;
        case 3:
          return FeetName::KHRYSAMERE_HG_FEET;
        default:
          return FeetName::_INVALID_VALUE;
      }
    }
    case BodyVariant::MIMIR_EBONIC_BODY:
    {
      switch (aRelativeFeetName)
      {
        case 0:
          return FeetName::MIMIR_EBONIC_BODY;
        case 1:
          return FeetName::MORE_SLIDERS_FOR_FEET;
        default:
          return FeetName::_INVALID_VALUE;
      }
    }
    case BodyVariant::_INVALID_VALUE:
    default:
    {
      return FeetName::_INVALID_VALUE;
    }
  }
}

BodyVariant DataLists::GetVariant(const BodyNameVersion& aBodyNameVersion)
{
  switch (aBodyNameVersion)
  {
    case BodyNameVersion::CBBE_3BA_3BBB_1_50:
    case BodyNameVersion::CBBE_3BA_3BBB_1_51_TO_1_55:
    case BodyNameVersion::CBBE_3BA_3BBB_2_02_TO_2_04:
    case BodyNameVersion::CBBE_3BA_3BBB_2_06:
    case BodyNameVersion::CBBE_3BA_3BBB_2_11_AND_2_12:
    case BodyNameVersion::CBBE_3BA_3BBB_2_13_AND_2_14:
      return BodyVariant::CBBE_3BA_3BBB;
    case BodyNameVersion::BHUNP_3BBB_2_20:
    case BodyNameVersion::BHUNP_3BBB_2_25:
    case BodyNameVersion::BHUNP_3BBB_2_30:
    case BodyNameVersion::BHUNP_3BBB_2_31:
    case BodyNameVersion::BHUNP_3BBB_2_35_TO_2_41:
      return BodyVariant::BHUNP_3BBB;
    case BodyNameVersion::BHUNP_3BBB_ADVANCED_2_20:
    case BodyNameVersion::BHUNP_3BBB_ADVANCED_2_25:
    case BodyNameVersion::BHUNP_3BBB_ADVANCED_2_30:
    case BodyNameVersion::BHUNP_3BBB_ADVANCED_2_31:
    case BodyNameVersion::BHUNP_3BBB_ADVANCED_2_35_TO_2_41:
      return BodyVariant::BHUNP_3BBB_ADVANCED;
    case BodyNameVersion::BHUNP_3BBB_ADVANCED_VER_2_2_20:
    case BodyNameVersion::BHUNP_3BBB_ADVANCED_VER_2_2_25:
    case BodyNameVersion::BHUNP_3BBB_ADVANCED_VER_2_2_30:
    case BodyNameVersion::BHUNP_3BBB_ADVANCED_VER_2_2_31:
    case BodyNameVersion::BHUNP_3BBB_ADVANCED_VER_2_2_35_TO_2_41:
      return BodyVariant::BHUNP_3BBB_ADVANCED_VER_2;
    case BodyNameVersion::BHUNP_BBP_2_20:
    case BodyNameVersion::BHUNP_BBP_2_25:
    case BodyNameVersion::BHUNP_BBP_2_30:
    case BodyNameVersion::BHUNP_BBP_2_31:
    case BodyNameVersion::BHUNP_BBP_2_35_TO_2_41:
      return BodyVariant::BHUNP_BBP;
    case BodyNameVersion::BHUNP_BBP_ADVANCED_2_20:
    case BodyNameVersion::BHUNP_BBP_ADVANCED_2_25:
    case BodyNameVersion::BHUNP_BBP_ADVANCED_2_30:
    case BodyNameVersion::BHUNP_BBP_ADVANCED_2_31:
    case BodyNameVersion::BHUNP_BBP_ADVANCED_2_35_TO_2_41:
      return BodyVariant::BHUNP_BBP_ADVANCED;
    case BodyNameVersion::BHUNP_TBBP_2_20:
    case BodyNameVersion::BHUNP_TBBP_2_25:
    case BodyNameVersion::BHUNP_TBBP_2_30:
    case BodyNameVersion::BHUNP_TBBP_2_31:
    case BodyNameVersion::BHUNP_TBBP_2_35_TO_2_41:
      return BodyVariant::BHUNP_TBBP;
    case BodyNameVersion::BHUNP_TBBP_ADVANCED_2_20:
    case BodyNameVersion::BHUNP_TBBP_ADVANCED_2_25:
    case BodyNameVersion::BHUNP_TBBP_ADVANCED_2_30:
    case BodyNameVersion::BHUNP_TBBP_ADVANCED_2_31:
    case BodyNameVersion::BHUNP_TBBP_ADVANCED_2_35_TO_2_41:
      return BodyVariant::BHUNP_TBBP_ADVANCED;
    case BodyNameVersion::BHUNP_3BBB_ADVANCED_VER_2_NEVERNUDE_2_25:
    case BodyNameVersion::BHUNP_3BBB_ADVANCED_VER_2_NEVERNUDE_2_30:
      return BodyVariant::BHUNP_3BBB_ADVANCED_VER_2_NEVERNUDE;
    case BodyNameVersion::CBBE_SMP_3BBB_1_2_0:
      return BodyVariant::CBBE_SMP_3BBB;
    case BodyNameVersion::COCO_BODY_CBBE_V4:
    case BodyNameVersion::COCO_BODY_CBBE_V6:
      return BodyVariant::COCO_BODY_CBBE;
    case BodyNameVersion::COCO_BODY_UUNP_V3:
    case BodyNameVersion::COCO_BODY_UUNP_V4:
    case BodyNameVersion::COCO_BODY_UUNP_V6:
      return BodyVariant::COCO_BODY_UUNP;
    case BodyNameVersion::MIMIR_EBONIC_BODY_1_2:
    case BodyNameVersion::MIMIR_EBONIC_BODY_1_2_FOOT_SEAMS_FIX:
      return BodyVariant::MIMIR_EBONIC_BODY;
    case BodyNameVersion::_INVALID_VALUE:
    default:
      return BodyVariant::_INVALID_VALUE;
  }
}

BodyVariant DataLists::GetVariant(const BodyName& aBodyName, const int aRelativeVersion, const int aRelativeVariant)
{
  switch (aBodyName)
  {
    case BodyName::CBBE_3BA_3BBB:
      return BodyVariant::CBBE_3BA_3BBB;
    case BodyName::BHUNP_UUNP_NEXT_GENERATION:
      // If not 2.25 and 2.30 but trying to use nevernude key, this is invalid
      if (aRelativeVersion != 1 && aRelativeVersion != 2 && aRelativeVariant > 6)
      {
        return BodyVariant::_INVALID_VALUE;
      }
      return static_cast<BodyVariant>(aRelativeVariant + 1);
    case BodyName::CBBE_SMP_3BBB:
      return BodyVariant::CBBE_SMP_3BBB;
    case BodyName::COCO_BODY_CBBE:
      return BodyVariant::COCO_BODY_CBBE;
    case BodyName::COCO_BODY_UUNP:
      return BodyVariant::COCO_BODY_UUNP;
    case BodyName::MIMIR_EBONIC_BODY:
      return BodyVariant::MIMIR_EBONIC_BODY;
    case BodyName::_INVALID_VALUE:
    default:
      return BodyVariant::_INVALID_VALUE;
  }
}

FeetVariant DataLists::GetVariant(const FeetNameVersion& aFeetNameVersion)
{
  switch (aFeetNameVersion)
  {
    case FeetNameVersion::CBBE_1_6_1:
      return FeetVariant::CBBE;
    case FeetNameVersion::BHUNP_3BBB_2_20:
    case FeetNameVersion::BHUNP_3BBB_2_25:
    case FeetNameVersion::BHUNP_3BBB_2_30:
    case FeetNameVersion::BHUNP_3BBB_2_31:
    case FeetNameVersion::BHUNP_3BBB_2_35_TO_2_41:
      return FeetVariant::BHUNP;
    case FeetNameVersion::CBBE_3BA_3BBB_1_50:
    case FeetNameVersion::CBBE_3BA_3BBB_1_51_TO_1_55:
    case FeetNameVersion::CBBE_3BA_3BBB_2_02_TO_2_04:
    case FeetNameVersion::CBBE_3BA_3BBB_2_06:
    case FeetNameVersion::CBBE_3BA_3BBB_2_11_AND_2_12:
    case FeetNameVersion::CBBE_3BA_3BBB_2_13_AND_2_14:
      return FeetVariant::CBBE_3BA_3BBB;
    case FeetNameVersion::COCO_BODY_CBBE_V4:
    case FeetNameVersion::COCO_BODY_CBBE_V6:
      return FeetVariant::COCO_BODY_CBBE;
    case FeetNameVersion::COCO_BODY_UUNP_V3:
    case FeetNameVersion::COCO_BODY_UUNP_V4:
    case FeetNameVersion::COCO_BODY_UUNP_V6:
      return FeetVariant::COCO_BODY_UUNP;
    case FeetNameVersion::MIMIR_EBONIC_BODY_1_2:
    case FeetNameVersion::MIMIR_EBONIC_BODY_1_2_FOOT_SEAMS_FIX:
      return FeetVariant::MIMIR_EBONIC_BODY;
    case FeetNameVersion::MORE_SLIDERS_FOR_FEET_NORMAL_CBBE_1_0:
      return FeetVariant::MORE_SLIDERS_FOR_FEET_NORMAL_CBBE;
    case FeetNameVersion::MORE_SLIDERS_FOR_FEET_NORMAL_BHUNP_1_1:
      return FeetVariant::MORE_SLIDERS_FOR_FEET_NORMAL_BHUNP;
    case FeetNameVersion::MORE_SLIDERS_FOR_FEET_HIGH_HEELS_CBBE_1_0:
      return FeetVariant::MORE_SLIDERS_FOR_FEET_HIGH_HEELS_CBBE;
    case FeetNameVersion::MORE_SLIDERS_FOR_FEET_HIGH_HEELS_BHUNP_1_1:
      return FeetVariant::MORE_SLIDERS_FOR_FEET_HIGH_HEELS_BHUNP;
    case FeetNameVersion::HG_FEET_1:
      return FeetVariant::HG_FEET;
    case FeetNameVersion::KHRYSAMERE_HG_FEET_1_0:
    case FeetNameVersion::KHRYSAMERE_HG_FEET_1_3_3:
    case FeetNameVersion::KHRYSAMERE_HG_FEET_1_4:
    case FeetNameVersion::KHRYSAMERE_HG_FEET_1_5:
    case FeetNameVersion::KHRYSAMERE_HG_FEET_1_6:
      return FeetVariant::KHRYSAMERE_HG_FEET;
    case FeetNameVersion::_INVALID_VALUE:
    default:
      return FeetVariant::_INVALID_VALUE;
  }
}

FeetVariant DataLists::GetVariant(const FeetName& aFeetName, const int aRelativeVersion, const int aRelativeVariant, const bool aIsCBBEBody)
{
  Q_UNUSED(aRelativeVersion);

  switch (aFeetName)
  {
    case FeetName::CBBE:
      return FeetVariant::CBBE;
    case FeetName::BHUNP:
      return FeetVariant::BHUNP;
    case FeetName::CBBE_3BA_3BBB:
      return FeetVariant::CBBE_3BA_3BBB;
    case FeetName::COCO_BODY:
      if (aIsCBBEBody)
      {
        return FeetVariant::COCO_BODY_CBBE;
      }
      return FeetVariant::COCO_BODY_UUNP;
    case FeetName::MIMIR_EBONIC_BODY:
      return FeetVariant::MIMIR_EBONIC_BODY;
    case FeetName::MORE_SLIDERS_FOR_FEET:
      if (aIsCBBEBody)
      {
        if (aRelativeVariant == 0)
        {
          return FeetVariant::MORE_SLIDERS_FOR_FEET_NORMAL_CBBE;
        }
        return FeetVariant::MORE_SLIDERS_FOR_FEET_HIGH_HEELS_CBBE;
      }
      if (aRelativeVariant == 0)
      {
        return FeetVariant::MORE_SLIDERS_FOR_FEET_NORMAL_BHUNP;
      }
      return FeetVariant::MORE_SLIDERS_FOR_FEET_HIGH_HEELS_BHUNP;
    case FeetName::HG_FEET:
      return FeetVariant::HG_FEET;
    case FeetName::KHRYSAMERE_HG_FEET:
      return FeetVariant::KHRYSAMERE_HG_FEET;
    default:
      return FeetVariant::_INVALID_VALUE;
  }
}

BodyNameVersion DataLists::GetBodyNameVersion(const BodyVariant& aBodyVariant, const int aRelativeVersion)
{
  // Ignore the cases where the checkboxes have no index selected
  if (aRelativeVersion == -1)
    return BodyNameVersion::_INVALID_VALUE;

  // Avoid having an undefined version selected
  if (aRelativeVersion >= DataLists::GetBodyVersionsList(DataLists::GetName(aBodyVariant)).size())
    return BodyNameVersion::_INVALID_VALUE;

  const auto lOffsetFirstKey{static_cast<int>(DataLists::GetFirstKey(aBodyVariant))};

  // Nevernude is only available in v.25 and 2.30
  if (aBodyVariant == BodyVariant::BHUNP_3BBB_ADVANCED_VER_2_NEVERNUDE)
    return static_cast<BodyNameVersion>(lOffsetFirstKey + aRelativeVersion - 2);

  return static_cast<BodyNameVersion>(lOffsetFirstKey + aRelativeVersion);
}

BodyNameVersion DataLists::GetBodyNameVersion(const BodyName& aBodyName, const int aRelativeVersion, const int aRelativeVariant)
{
  // Ignore the cases where the checkboxes have no index selected
  if (aRelativeVariant == -1)
    return BodyNameVersion::_INVALID_VALUE;

  const auto lBodyVariant{DataLists::GetVariant(aBodyName, aRelativeVersion, aRelativeVariant)};

  return DataLists::GetBodyNameVersion(lBodyVariant, aRelativeVersion);
}

FeetNameVersion DataLists::GetFeetNameVersion(const FeetVariant& aFeetVariant, const int aRelativeVersion, const bool aIsCBBEBody)
{
  // Ignore the cases where the checkboxes have no index selected
  if (aRelativeVersion == -1)
    return FeetNameVersion::_INVALID_VALUE;

  // Avoid having an undefined version selected
  if (aRelativeVersion >= DataLists::GetFeetVersionsList(DataLists::GetName(aFeetVariant), aIsCBBEBody).size())
    return FeetNameVersion::_INVALID_VALUE;

  auto lFeetName{DataLists::GetName(aFeetVariant)};
  auto lOffsetFirstKey{static_cast<int>(DataLists::GetFirstKey(aFeetVariant))};

  switch (lFeetName)
  {
    case FeetName::CBBE:
    case FeetName::BHUNP:
    case FeetName::CBBE_3BA_3BBB:
    case FeetName::COCO_BODY:
    case FeetName::MIMIR_EBONIC_BODY:
    case FeetName::MORE_SLIDERS_FOR_FEET:
    case FeetName::HG_FEET:
    case FeetName::KHRYSAMERE_HG_FEET:
      return static_cast<FeetNameVersion>(lOffsetFirstKey + aRelativeVersion);
  }

  return FeetNameVersion::_INVALID_VALUE;
}

FeetNameVersion DataLists::GetFeetNameVersion(const FeetName& aFeetName, const int aRelativeVersion, const int aRelativeVariant, const bool aIsCBBEBody)
{
  // Ignore the cases where the checkboxes have no index selected
  if (aRelativeVariant == -1)
    return FeetNameVersion::_INVALID_VALUE;

  const auto lFeetVariant{DataLists::GetVariant(aFeetName, aRelativeVersion, aRelativeVariant, aIsCBBEBody)};

  return DataLists::GetFeetNameVersion(lFeetVariant, aRelativeVersion, aIsCBBEBody);
}

int DataLists::GetVersionIndex(const BodyNameVersion& aBodyNameVersion)
{
  if (aBodyNameVersion == BodyNameVersion::_INVALID_VALUE)
  {
    return -1;
  }

  // Calculate an offset if the first version number of a variant is not the version of the base body itself
  auto lVersionNumberOffset{0};
  auto lBodyVariant{DataLists::GetVariant(aBodyNameVersion)};
  if (lBodyVariant == BodyVariant::BHUNP_3BBB_ADVANCED_VER_2_NEVERNUDE)
  {
    // 2.20 is the first BHUNP version, while 2.25 is the first version if the nevernude variant
    lVersionNumberOffset = 1;
  }

  auto lOffsetFirstKey{static_cast<int>(DataLists::GetFirstKey(DataLists::GetVariant(aBodyNameVersion)))};
  return (static_cast<int>(aBodyNameVersion) - lOffsetFirstKey + lVersionNumberOffset);
}

int DataLists::GetVariantIndex(const BodyNameVersion& aBodyNameVersion)
{
  if (aBodyNameVersion == BodyNameVersion::_INVALID_VALUE)
  {
    return -1;
  }

  auto lBodyVariant{DataLists::GetVariant(aBodyNameVersion)};
  switch (lBodyVariant)
  {
    case BodyVariant::CBBE_3BA_3BBB:
    case BodyVariant::CBBE_SMP_3BBB:
    case BodyVariant::COCO_BODY_CBBE:
    case BodyVariant::COCO_BODY_UUNP:
    case BodyVariant::MIMIR_EBONIC_BODY:
      return 0;
    case BodyVariant::BHUNP_3BBB:
    case BodyVariant::BHUNP_3BBB_ADVANCED:
    case BodyVariant::BHUNP_3BBB_ADVANCED_VER_2:
    case BodyVariant::BHUNP_BBP:
    case BodyVariant::BHUNP_BBP_ADVANCED:
    case BodyVariant::BHUNP_TBBP:
    case BodyVariant::BHUNP_TBBP_ADVANCED:
    case BodyVariant::BHUNP_3BBB_ADVANCED_VER_2_NEVERNUDE:
      return static_cast<int>(lBodyVariant) - 1;
    default:
      return -1;
  }
}

int DataLists::GetNameIndex(const BodyVariant& aBodyVariant, const FeetNameVersion& aFeetNameVersion)
{
  auto lFeetName{DataLists::GetName(aFeetNameVersion)};

  switch (aBodyVariant)
  {
    case BodyVariant::CBBE_3BA_3BBB:
    {
      switch (lFeetName)
      {
        case FeetName::CBBE_3BA_3BBB:
          return 0;
        case FeetName::MORE_SLIDERS_FOR_FEET:
          return 1;
        default:
          return -1;
      }
    }
    case BodyVariant::BHUNP_3BBB:
    case BodyVariant::BHUNP_3BBB_ADVANCED:
    case BodyVariant::BHUNP_3BBB_ADVANCED_VER_2:
    case BodyVariant::BHUNP_BBP:
    case BodyVariant::BHUNP_BBP_ADVANCED:
    case BodyVariant::BHUNP_TBBP:
    case BodyVariant::BHUNP_TBBP_ADVANCED:
    case BodyVariant::BHUNP_3BBB_ADVANCED_VER_2_NEVERNUDE:
    {
      switch (lFeetName)
      {
        case FeetName::BHUNP:
          return 0;
        case FeetName::MORE_SLIDERS_FOR_FEET:
          return 1;
        case FeetName::HG_FEET:
          return 2;
        case FeetName::KHRYSAMERE_HG_FEET:
          return 3;
        default:
          return -1;
      }
    }
    case BodyVariant::CBBE_SMP_3BBB:
    {
      switch (lFeetName)
      {
        case FeetName::CBBE:
          return 0;
        case FeetName::MORE_SLIDERS_FOR_FEET:
          return 1;
        default:
          return -1;
      }
    }
    case BodyVariant::COCO_BODY_CBBE:
    {
      switch (lFeetName)
      {
        case FeetName::COCO_BODY:
          return 0;
        case FeetName::MORE_SLIDERS_FOR_FEET:
          return 1;
        default:
          return -1;
      }
    }
    case BodyVariant::COCO_BODY_UUNP:
    {
      switch (lFeetName)
      {
        case FeetName::COCO_BODY:
          return 0;
        default:
          return -1;
      }
    }
    case BodyVariant::MIMIR_EBONIC_BODY:
    {
      switch (lFeetName)
      {
        case FeetName::MIMIR_EBONIC_BODY:
          return 0;
        case FeetName::MORE_SLIDERS_FOR_FEET:
          return 1;
        default:
          return -1;
      }
    }
    case BodyVariant::_INVALID_VALUE:
    default:
    {
      return -1;
    }
  }
}

int DataLists::GetVersionIndex(const FeetNameVersion& aFeetNameVersion)
{
  if (aFeetNameVersion == FeetNameVersion::_INVALID_VALUE)
  {
    return -1;
  }

  auto lOffsetFirstKey{static_cast<int>(DataLists::GetFirstKey(DataLists::GetVariant(aFeetNameVersion)))};
  return (static_cast<int>(aFeetNameVersion) - lOffsetFirstKey);
}

int DataLists::GetVariantIndex(const FeetNameVersion& aFeetNameVersion)
{
  if (aFeetNameVersion == FeetNameVersion::_INVALID_VALUE)
  {
    return -1;
  }

  auto lFeetVariant{DataLists::GetVariant(aFeetNameVersion)};
  switch (lFeetVariant)
  {
    case FeetVariant::CBBE:
    case FeetVariant::BHUNP:
    case FeetVariant::CBBE_3BA_3BBB:
    case FeetVariant::COCO_BODY_CBBE:
    case FeetVariant::COCO_BODY_UUNP:
    case FeetVariant::MIMIR_EBONIC_BODY:
    case FeetVariant::HG_FEET:
    case FeetVariant::KHRYSAMERE_HG_FEET:
    case FeetVariant::MORE_SLIDERS_FOR_FEET_NORMAL_CBBE:
    case FeetVariant::MORE_SLIDERS_FOR_FEET_NORMAL_BHUNP:
      return 0;
    case FeetVariant::MORE_SLIDERS_FOR_FEET_HIGH_HEELS_CBBE:
    case FeetVariant::MORE_SLIDERS_FOR_FEET_HIGH_HEELS_BHUNP:
      return 1;
    default:
      return -1;
  }
}

QString DataLists::GetVersionString(const BodyNameVersion& aBodyNameVersion)
{
  return DataLists::GetBodyVersionsList(DataLists::GetName(aBodyNameVersion)).at(DataLists::GetVersionIndex(aBodyNameVersion));
}

QString DataLists::GetVersionString(const BodyNameVersion& aBodyNameVersion, const FeetNameVersion& aFeetNameVersion)
{
  return DataLists::GetFeetVersionsList(DataLists::GetName(aFeetNameVersion), Utils::IsCBBEBasedBody(DataLists::GetVariant(aBodyNameVersion))).at(DataLists::GetVersionIndex(aFeetNameVersion));
}

BodyNameVersion DataLists::GetFirstKey(const BodyVariant& aBodyVariant)
{
  // Return the lowest index of BodyNameVersion of a given BodyVariant
  switch (aBodyVariant)
  {
    case BodyVariant::CBBE_3BA_3BBB:
      return BodyNameVersion::CBBE_3BA_3BBB_1_50;
    case BodyVariant::BHUNP_3BBB:
      return BodyNameVersion::BHUNP_3BBB_2_20;
    case BodyVariant::BHUNP_3BBB_ADVANCED:
      return BodyNameVersion::BHUNP_3BBB_ADVANCED_2_20;
    case BodyVariant::BHUNP_3BBB_ADVANCED_VER_2:
      return BodyNameVersion::BHUNP_3BBB_ADVANCED_VER_2_2_20;
    case BodyVariant::BHUNP_BBP:
      return BodyNameVersion::BHUNP_BBP_2_20;
    case BodyVariant::BHUNP_BBP_ADVANCED:
      return BodyNameVersion::BHUNP_BBP_ADVANCED_2_20;
    case BodyVariant::BHUNP_TBBP:
      return BodyNameVersion::BHUNP_TBBP_2_20;
    case BodyVariant::BHUNP_TBBP_ADVANCED:
      return BodyNameVersion::BHUNP_TBBP_ADVANCED_2_20;
    case BodyVariant::BHUNP_3BBB_ADVANCED_VER_2_NEVERNUDE:
      return BodyNameVersion::BHUNP_3BBB_ADVANCED_VER_2_NEVERNUDE_2_25;
    case BodyVariant::CBBE_SMP_3BBB:
      return BodyNameVersion::CBBE_SMP_3BBB_1_2_0;
    case BodyVariant::COCO_BODY_CBBE:
      return BodyNameVersion::COCO_BODY_CBBE_V4;
    case BodyVariant::COCO_BODY_UUNP:
      return BodyNameVersion::COCO_BODY_UUNP_V3;
    case BodyVariant::MIMIR_EBONIC_BODY:
      return BodyNameVersion::MIMIR_EBONIC_BODY_1_2;
    case BodyVariant::_INVALID_VALUE:
    default:
      return BodyNameVersion::_INVALID_VALUE;
  }
}

FeetNameVersion DataLists::GetFirstKey(const FeetVariant& aFeetVariant)
{
  switch (aFeetVariant)
  {
    case FeetVariant::CBBE:
      return FeetNameVersion::CBBE_1_6_1;
    case FeetVariant::BHUNP:
      return FeetNameVersion::BHUNP_3BBB_2_20;
    case FeetVariant::CBBE_3BA_3BBB:
      return FeetNameVersion::CBBE_3BA_3BBB_1_50;
    case FeetVariant::COCO_BODY_CBBE:
      return FeetNameVersion::COCO_BODY_CBBE_V4;
    case FeetVariant::COCO_BODY_UUNP:
      return FeetNameVersion::COCO_BODY_UUNP_V3;
    case FeetVariant::MIMIR_EBONIC_BODY:
      return FeetNameVersion::MIMIR_EBONIC_BODY_1_2;
    case FeetVariant::MORE_SLIDERS_FOR_FEET_NORMAL_CBBE:
      return FeetNameVersion::MORE_SLIDERS_FOR_FEET_NORMAL_CBBE_1_0;
    case FeetVariant::MORE_SLIDERS_FOR_FEET_NORMAL_BHUNP:
      return FeetNameVersion::MORE_SLIDERS_FOR_FEET_NORMAL_BHUNP_1_1;
    case FeetVariant::MORE_SLIDERS_FOR_FEET_HIGH_HEELS_CBBE:
      return FeetNameVersion::MORE_SLIDERS_FOR_FEET_HIGH_HEELS_CBBE_1_0;
    case FeetVariant::MORE_SLIDERS_FOR_FEET_HIGH_HEELS_BHUNP:
      return FeetNameVersion::MORE_SLIDERS_FOR_FEET_HIGH_HEELS_BHUNP_1_1;
    case FeetVariant::HG_FEET:
      return FeetNameVersion::HG_FEET_1;
    case FeetVariant::KHRYSAMERE_HG_FEET:
      return FeetNameVersion::KHRYSAMERE_HG_FEET_1_0;
    case FeetVariant::_INVALID_VALUE:
    default:
      return FeetNameVersion::_INVALID_VALUE;
  }
}

QString DataLists::GetQRCPathForRessource(const BodyNameVersion& aBodyNameVersion, const FeetNameVersion& aFeetNameVersion, const BodyPartType& aRessourceType)
{
  QString lRessourceType;
  QString lRessourceName;
  QString lRessourceVersion;

  switch (aRessourceType)
  {
    case BodyPartType::BODY:
    {
      lRessourceType = "body";
      lRessourceName = DataLists::GetQRCRessourceName(DataLists::GetVariant(aBodyNameVersion));
      lRessourceVersion = DataLists::GetVersionString(aBodyNameVersion);
      break;
    }
    case BodyPartType::FEET:
    {
      lRessourceType = "feet";
      lRessourceName = DataLists::GetQRCRessourceName(DataLists::GetVariant(aBodyNameVersion), aFeetNameVersion);
      lRessourceVersion = DataLists::GetVersionString(aBodyNameVersion, aFeetNameVersion);
      break;
    }
    case BodyPartType::HANDS:
    {
      lRessourceType = "hands";
      lRessourceVersion = DataLists::GetVersionString(aBodyNameVersion);

      auto lBodyName{DataLists::GetName(aBodyNameVersion)};
      switch (lBodyName)
      {
        case BodyName::CBBE_3BA_3BBB:
          lRessourceName = QString("cbbe 3bbb 3ba");
          break;
        case BodyName::BHUNP_UUNP_NEXT_GENERATION:
          lRessourceName = QString("bhunp");
          break;
        case BodyName::CBBE_SMP_3BBB:
          lRessourceName = QString("cbbe");
          lRessourceVersion = QString("1.6.1"); // Default CBBE hands
          break;
        case BodyName::COCO_BODY_CBBE:
          lRessourceName = QString("coco body cbbe");
          break;
        case BodyName::COCO_BODY_UUNP:
          lRessourceName = QString("coco body uunp");
          break;
        case BodyName::MIMIR_EBONIC_BODY:
          lRessourceName = QString("mimir ebonic body");
          break;
        case BodyName::_INVALID_VALUE:
        default:
          lRessourceVersion = QString();
          break;
      }

      break;
    }
    case BodyPartType::BEAST_HANDS:
    {
      // For the beast hands, use the default CBBE v.1.6.1 beast hands
      if (Utils::IsCBBEBasedBody(aBodyNameVersion))
      {
        return QString(":/presets/beast_hands/cbbe/1.6.1");
      }

      // UNP-based bodies do not have beast hands compatibility
      return QString();
    }
  }

  return QString(":/presets/%1/%2/%3").arg(lRessourceType, lRessourceName, lRessourceVersion);
}

std::pair<int, int> DataLists::GetSplittedNameVersionFromBodyVersionCompatibility(const int aBodyVersion)
{
  switch (aBodyVersion)
  {
    case 0: // CBBE_3BBB_3BA_1_50
    case 1: // CBBE_3BBB_3BA_1_51_TO_1_55
      return std::pair<int, int>(static_cast<int>(0), static_cast<int>(aBodyVersion));
    case 17: // CBBE_3BBB_3BA_2_02_TO_2_04
      return std::pair<int, int>(static_cast<int>(0), 2);
    case 18: // CBBE_3BBB_3BA_2_06
      return std::pair<int, int>(static_cast<int>(0), 3);
    case 2: // CBBE_SMP_3BBB_1_2_0
      return std::pair<int, int>(static_cast<int>(1), 0);
    case 3: // BHUNP_3BBB_2_20:
    case 4: // BHUNP_3BBB_ADVANCED_2_20:
    case 5: // BHUNP_3BBB_ADVANCED_VER_2_2_20:
    case 6: // BHUNP_BBP_2_20:
    case 7: // BHUNP_BBP_ADVANCED_2_20:
    case 8: // BHUNP_TBBP_2_20:
    case 9: // BHUNP_TBBP_ADVANCED_2_20:
      return std::pair<int, int>(static_cast<int>(aBodyVersion) - 1, 0);
    case 10: // BHUNP_3BBB_2_25:
    case 11: // BHUNP_3BBB_ADVANCED_2_25:
    case 12: // BHUNP_3BBB_ADVANCED_VER_2_2_25:
    case 13: // BHUNP_BBP_2_25:
    case 14: // BHUNP_BBP_ADVANCED_2_25:
    case 15: // BHUNP_TBBP_2_25:
    case 16: // BHUNP_TBBP_ADVANCED_2_25:
      return std::pair<int, int>(static_cast<int>(aBodyVersion) - 8, 1);
    case 19: // BHUNP_3BBB_2_30:
    case 20: // BHUNP_3BBB_ADVANCED_2_30:
    case 21: // BHUNP_3BBB_ADVANCED_VER_2_2_30:
    case 22: // BHUNP_BBP_2_30:
    case 23: // BHUNP_BBP_ADVANCED_2_30:
    case 24: // BHUNP_TBBP_2_30:
    case 25: // BHUNP_TBBP_ADVANCED_2_30:
      return std::pair<int, int>(static_cast<int>(aBodyVersion) - 17, 2);
    case 26: // BHUNP_3BBB_ADVANCED_VER_2_NEVERNUDE_2_25
    case 27: // BHUNP_3BBB_ADVANCED_VER_2_NEVERNUDE_2_30
      return std::pair<int, int>(static_cast<int>(9), static_cast<int>(aBodyVersion) - 26);
    case 28: // BHUNP_3BBB_2_31:
    case 29: // BHUNP_3BBB_ADVANCED_2_31:
    case 30: // BHUNP_3BBB_ADVANCED_VER_2_2_31:
    case 31: // BHUNP_BBP_2_31:
    case 32: // BHUNP_BBP_ADVANCED_2_31:
    case 33: // BHUNP_TBBP_2_31:
    case 34: // BHUNP_TBBP_ADVANCED_2_31:
      return std::pair<int, int>(static_cast<int>(aBodyVersion) - 26, 3);
    case 35: // BHUNP_3BBB_2_35_TO_2_39:
    case 36: // BHUNP_3BBB_ADVANCED_2_35_TO_2_39:
    case 37: // BHUNP_3BBB_ADVANCED_VER_2_2_35_TO_2_39:
    case 38: // BHUNP_BBP_2_35_TO_2_39:
    case 39: // BHUNP_BBP_ADVANCED_2_35_TO_2_39:
    case 40: // BHUNP_TBBP_2_35_TO_2_39:
    case 41: // BHUNP_TBBP_ADVANCED_2_35_TO_2_39:
      return std::pair<int, int>(static_cast<int>(aBodyVersion) - 33, 4);
    case 42: // MIMIR_EBONIC_BODY_1_2:
    case 43: // MIMIR_EBONIC_BODY_1_2_FOOT_SEAMS_FIX:
      return std::pair<int, int>(static_cast<int>(BodyName::MIMIR_EBONIC_BODY), static_cast<int>(aBodyVersion) - 42);
    default:
      return std::pair<int, int>(-1, -1);
  }
}

std::pair<BodyNameVersion, FeetNameVersion> DataLists::ReadBodyFeetModsCompatibility(const int aBodyName, const int aBodyVersion, const int aFeetIndex)
{
  // Body compatibility
  auto lBodyVariant{BodyVariant::_INVALID_VALUE};

  switch (aBodyName)
  {
    case 0:
      lBodyVariant = BodyVariant::CBBE_3BA_3BBB;
      break;
    case 1:
      lBodyVariant = BodyVariant::CBBE_SMP_3BBB;
      break;
    case 2:
      lBodyVariant = BodyVariant::BHUNP_3BBB;
      break;
    case 3:
      lBodyVariant = BodyVariant::BHUNP_3BBB_ADVANCED;
      break;
    case 4:
      lBodyVariant = BodyVariant::BHUNP_3BBB_ADVANCED_VER_2;
      break;
    case 5:
      lBodyVariant = BodyVariant::BHUNP_BBP;
      break;
    case 6:
      lBodyVariant = BodyVariant::BHUNP_BBP_ADVANCED;
      break;
    case 7:
      lBodyVariant = BodyVariant::BHUNP_TBBP;
      break;
    case 8:
      lBodyVariant = BodyVariant::BHUNP_TBBP_ADVANCED;
      break;
    case 9:
      lBodyVariant = BodyVariant::BHUNP_3BBB_ADVANCED_VER_2_NEVERNUDE;
      break;
    case 10:
      lBodyVariant = BodyVariant::MIMIR_EBONIC_BODY;
      break;
    default:
      // Do nothing
      break;
  }

  // Feet compatibility
  auto lFeetToSet{FeetNameVersion::_INVALID_VALUE};
  if (Utils::IsCBBEBasedBody(lBodyVariant))
  {
    switch (aFeetIndex)
    {
      // Default
      case 0:
        if (lBodyVariant == BodyVariant::CBBE_3BA_3BBB)
        {
          lFeetToSet = DataLists::GetFirstKey(FeetVariant::CBBE_3BA_3BBB);
        }
        else if (lBodyVariant == BodyVariant::CBBE_SMP_3BBB)
        {
          lFeetToSet = DataLists::GetFirstKey(FeetVariant::CBBE);
        }
        else if (lBodyVariant == BodyVariant::MIMIR_EBONIC_BODY)
        {
          lFeetToSet = DataLists::GetFirstKey(FeetVariant::MIMIR_EBONIC_BODY);
        }
        break;
      // More Sliders for Feet - Normal
      case 1:
        lFeetToSet = DataLists::GetFirstKey(FeetVariant::MORE_SLIDERS_FOR_FEET_NORMAL_CBBE);
        break;
      // More Sliders for Feet - High Heels
      case 2:
        lFeetToSet = DataLists::GetFirstKey(FeetVariant::MORE_SLIDERS_FOR_FEET_HIGH_HEELS_CBBE);
        break;
      default:
        // Do nothing
        break;
    }
  }
  else
  {
    switch (aFeetIndex)
    {
      // Default
      case 0:
        lFeetToSet = DataLists::GetFirstKey(FeetVariant::BHUNP);
        break;
      // More Sliders for Feet - Normal
      case 1:
        lFeetToSet = DataLists::GetFirstKey(FeetVariant::MORE_SLIDERS_FOR_FEET_NORMAL_BHUNP);
        break;
      // More Sliders for Feet - High Heels
      case 2:
        lFeetToSet = DataLists::GetFirstKey(FeetVariant::MORE_SLIDERS_FOR_FEET_HIGH_HEELS_BHUNP);
        break;
      // HG Feet and Toes BHUNP SE - HGFeet UUNP
      case 3:
        lFeetToSet = DataLists::GetFirstKey(FeetVariant::HG_FEET);
        break;
      // Khrysamere HG Feet (BHUNP)
      case 4:
      //Khrysamere HG Feet (Claws) (BHUNP)
      case 5:
        lFeetToSet = DataLists::GetFirstKey(FeetVariant::KHRYSAMERE_HG_FEET);
        break;
      default:
        // Do nothing
        break;
    }
  }

  // If the relative version is too high, take the first one available
  if (aBodyVersion >= DataLists::GetBodyVersionsList(DataLists::GetName(lBodyVariant)).size())
  {
    return std::make_pair(DataLists::GetBodyNameVersion(lBodyVariant, 0), lFeetToSet);
  }

  return std::make_pair(DataLists::GetBodyNameVersion(lBodyVariant, aBodyVersion), lFeetToSet);
}

QStringList DataLists::GetWindowOpeningModes()
{
  return QStringList{tr("Minimized"),
                     tr("Windowed"),
                     tr("Maximized")};
}

QStringList DataLists::GetDialogOpeningModes()
{
  return QStringList{tr("Windowed"),
                     tr("Maximized")};
}

QStringList DataLists::GetAppThemes()
{
  return QStringList{QString("Windows Vista"),
                     QString("Mitsuriou's Black Theme"),
                     QString("Mitsuriou's Dark Theme"),
                     QString("Mitsuriou's Light Theme"),
                     QString("Alexhuszagh's Breeze Dark"),
                     QString("Alexhuszagh's Breeze Light"),
                     QString("Paper Dark by 6788"),
                     QString("Paper Light by 6788"),
                     QString("Paper Black Mono by 6788"),
                     QString("Paper White Mono by 6788")};
}

QStringList DataLists::GetLanguages()
{
  QStringList lLanguages{QString("English"),
                         QString("Français"),
                         QString("繁体中文(台湾) - translation by Sephirothryan")};

  return lLanguages;
}

QStringList DataLists::GetAssistedConversionActions()
{
  return QStringList{tr("[ignore]"),
                     tr("Body mesh"),
                     tr("Feet mesh"),
                     tr("Hands mesh"),
                     tr("Skeleton mesh")};
}

QStringList DataLists::GetLastPathsKeys()
{
  return QStringList{"general",
                     "mainWindowOutput",
                     "batchConversionInput",
                     "batchConversionOutput",
                     "assistedConversionInput",
                     "retargetingToolInput",
                     "retargetingToolOutput",
                     "texturesAssistantInput",
                     "lastLoadedProject",
                     "lastSavedProject"};
}
