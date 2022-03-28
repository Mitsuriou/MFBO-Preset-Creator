#include "DataLists.h"
#include "Utils.h"

QStringList DataLists::GetBodyNamesList()
{
  return QStringList{QString("CBBE 3BA (3BBB) - by Acro"),
                     QString("BHUNP (UUNP Next Generation) SSE - by Bakafactory and Haeun"),
                     QString("CBBE SMP (3BBB) - by Ousnius"),
                     QString("COCO Body CBBE - by COCO"),
                     QString("COCO Body UUNP - by COCO"),
                     QString("Mimir Ebonic Body V.2 - by Medley"),
                     QString("BHUNP Lite - AKA ASDASDF Body - by Myst")};
}

QStringList DataLists::GetBodyVersionsList(const BodyName& aBodyName)
{
  switch (aBodyName)
  {
    case BodyName::_INVALID_VALUE:
    {
      return QStringList();
    }
    case BodyName::CBBE_3BA_3BBB:
    {
      return QStringList({QString("1.50"),
                          QString("1.51 to 1.55"),
                          QString("2.02 to 2.04"),
                          QString("2.06 - 2.11 - 2.12"),
                          QString("2.13 to 2.22 - 2.30 - 2.31")});
    }
    case BodyName::BHUNP_UUNP_NEXT_GENERATION:
    {
      return QStringList({QString("2.20"),
                          QString("2.25"),
                          QString("2.30"),
                          QString("2.31"),
                          QString("2.35 to 2.42"),
                          QString("3.00 to 3.02"),
                          QString("3.03"),
                          QString("3.04 and 3.05")});
    }
    case BodyName::CBBE_SMP_3BBB:
    {
      return QStringList(QString("1.2.0"));
    }
    case BodyName::COCO_BODY_CBBE:
    {
      return QStringList({QString("V4"), QString("V6"), QString("V6.8")});
    }
    case BodyName::COCO_BODY_UUNP:
    {
      return QStringList({QString("V3"), QString("V4"), QString("V6"), QString("V6.8")});
    }
    case BodyName::MIMIR_EBONIC_BODY:
    {
      return QStringList({QString("1.2"), QString("1.2 foot fix")});
    }
    case BodyName::BHUNP_LITE_ASDASDF:
    {
      return QStringList(QString("1.13"));
    }
  }

  return QStringList();
}

QStringList DataLists::GetBodyVariantsList(const BodyName& aBodyName, const int aRelativeVersion)
{
  switch (aBodyName)
  {
    case BodyName::_INVALID_VALUE:
    {
      return QStringList();
    }
    case BodyName::CBBE_3BA_3BBB:
    {
      // All the version numbers propose the variant below:
      QStringList lVariantsList{QString("CBBE 3BBB Body Amazing")};

      // Only for "2.13 to 2.22 - 2.30 - 2.31"
      if (aRelativeVersion == 4)
      {
        lVariantsList.append(QString("CBBE 3BBB Amazing NeverNude"));
        lVariantsList.append(QString("CBBE 3BBB Amazing Underwear"));
      }

      return lVariantsList;
    }
    case BodyName::BHUNP_UUNP_NEXT_GENERATION:
    {
      QStringList lVariantsList;

      // All the version numbers propose the variant below:
      lVariantsList.append(QString("BHUNP 3BBB"));
      lVariantsList.append(QString("BHUNP 3BBB Advanced"));
      lVariantsList.append(QString("BHUNP 3BBB Advanced Ver 2"));

      // Only for "3.00 to 3.02", "3.03" and "3.04 and 3.05"
      if (aRelativeVersion >= 5 && aRelativeVersion <= 7)
        lVariantsList.append(QString("BHUNP 3BBB Advanced Ver 3"));

      // All the version numbers propose the variant below:
      lVariantsList.append(QString("BHUNP BBP"));
      lVariantsList.append(QString("BHUNP BBP Advanced"));
      lVariantsList.append(QString("BHUNP TBBP"));
      lVariantsList.append(QString("BHUNP TBBP Advanced"));

      // If "2.20" or "2.31"
      if (aRelativeVersion == 0 || aRelativeVersion == 3)
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
    case BodyName::BHUNP_LITE_ASDASDF:
    {
      // All the version numbers propose the variants below:
      return QStringList{QString("ASDASDF 3BBB"),
                         QString("ASDASDF 3BBB - NO THIGH JIGGLE"),
                         QString("ASDASDF BBP"),
                         QString("ASDASDF BBP - NO THIGH JIGGLE"),
                         QString("ASDASDF TBBP"),
                         QString("ASDASDF TBBP - NO THIGH JIGGLE"),
                         QString("ASDASDF ALTERBODY 3BBB"),
                         QString("ASDASDF ALTERBODY 3BBB - NO THIGH JIGGLE"),
                         QString("ASDASDF ALTERBODY BBP"),
                         QString("ASDASDF ALTERBODY BBP - NO THIGH JIGGLE"),
                         QString("ASDASDF ALTERBODY TBBP"),
                         QString("ASDASDF ALTERBODY TBBP - NO THIGH JIGGLE")};
    }
  }

  return QStringList();
}

QString DataLists::GetQRCResourceName(const BodyVariant& aBodyVariant)
{
  switch (aBodyVariant)
  {
    case BodyVariant::_INVALID_VALUE:
      return QString();
    case BodyVariant::CBBE_3BA_3BBB:
      return QString("cbbe 3bbb 3ba");
    case BodyVariant::CBBE_3BA_3BBB_NEVERNUDE:
      return QString("cbbe 3bbb 3ba nevernude");
    case BodyVariant::CBBE_3BA_3BBB_UNDERWEAR:
      return QString("cbbe 3bbb 3ba underwear");
    case BodyVariant::BHUNP_3BBB:
      return QString("bhunp 3bbb");
    case BodyVariant::BHUNP_3BBB_ADVANCED:
      return QString("bhunp 3bbb advanced");
    case BodyVariant::BHUNP_3BBB_ADVANCED_VER_2:
      return QString("bhunp 3bbb advanced ver 2");
    case BodyVariant::BHUNP_3BBB_ADVANCED_VER_3:
      return QString("bhunp 3bbb advanced ver 3");
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
    case BodyVariant::ASDASDF_3BBB:
      return QString("asdasdf 3bbb");
    case BodyVariant::ASDASDF_3BBB_NO_THIGH_JIGGLE:
      return QString("asdasdf 3bbb - ntj");
    case BodyVariant::ASDASDF_BBP:
      return QString("asdasdf bbp");
    case BodyVariant::ASDASDF_BBP_NO_THIGH_JIGGLE:
      return QString("asdasdf bbp - ntj");
    case BodyVariant::ASDASDF_TBBP:
      return QString("asdasdf tbbp");
    case BodyVariant::ASDASDF_TBBP_NO_THIGH_JIGGLE:
      return QString("asdasdf tbbp - ntj");
    case BodyVariant::ASDASDF_ALTERBODY_3BBB:
      return QString("asdasdf alterbody 3bbb");
    case BodyVariant::ASDASDF_ALTERBODY_3BBB_NO_THIGH_JIGGLE:
      return QString("asdasdf alterbody 3bbb - ntj");
    case BodyVariant::ASDASDF_ALTERBODY_BBP:
      return QString("asdasdf alterbody bbp");
    case BodyVariant::ASDASDF_ALTERBODY_BBP_NO_THIGH_JIGGLE:
      return QString("asdasdf alterbody bbp - ntj");
    case BodyVariant::ASDASDF_ALTERBODY_TBBP:
      return QString("asdasdf alterbody tbbp");
    case BodyVariant::ASDASDF_ALTERBODY_TBBP_NO_THIGH_JIGGLE:
      return QString("asdasdf alterbody tbbp - ntj");
  }

  return QString();
}

QStringList DataLists::GetFeetNamesList(const BodyVariant& aBodyVariant)
{
  switch (aBodyVariant)
  {
    case BodyVariant::CBBE_3BA_3BBB:
    case BodyVariant::CBBE_3BA_3BBB_NEVERNUDE:
    case BodyVariant::CBBE_3BA_3BBB_UNDERWEAR:
    {
      return QStringList{QString("CBBE 3BBB Feet - by Acro"),
                         QString("More Sliders for Feet - by Balveric")};
    }
    case BodyVariant::BHUNP_3BBB:
    case BodyVariant::BHUNP_3BBB_ADVANCED:
    case BodyVariant::BHUNP_3BBB_ADVANCED_VER_2:
    case BodyVariant::BHUNP_3BBB_ADVANCED_VER_3:
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
    case BodyVariant::ASDASDF_3BBB:
    case BodyVariant::ASDASDF_3BBB_NO_THIGH_JIGGLE:
    case BodyVariant::ASDASDF_BBP:
    case BodyVariant::ASDASDF_BBP_NO_THIGH_JIGGLE:
    case BodyVariant::ASDASDF_TBBP:
    case BodyVariant::ASDASDF_TBBP_NO_THIGH_JIGGLE:
    case BodyVariant::ASDASDF_ALTERBODY_3BBB:
    case BodyVariant::ASDASDF_ALTERBODY_3BBB_NO_THIGH_JIGGLE:
    case BodyVariant::ASDASDF_ALTERBODY_BBP:
    case BodyVariant::ASDASDF_ALTERBODY_BBP_NO_THIGH_JIGGLE:
    case BodyVariant::ASDASDF_ALTERBODY_TBBP:
    case BodyVariant::ASDASDF_ALTERBODY_TBBP_NO_THIGH_JIGGLE:
    {
      return QStringList{QString("ASDASDF BHUNP Feet - by Myst"),
                         QString("More Sliders for Feet - by Balveric"),
                         QString("HG Feet and Toes UNP - by SunJeong"),
                         QString("K Feet - by Khrysamere")};
    }
    case BodyVariant::_INVALID_VALUE:
    {
      return QStringList();
    }
  }

  return QStringList();
}

QStringList DataLists::GetFeetVersionsList(const FeetName& aFeetName, const bool aIsCBBEBody)
{
  switch (aFeetName)
  {
    case FeetName::_INVALID_VALUE:
      return QStringList();
    case FeetName::CBBE:
      return QStringList{QString("1.6.1")};
    case FeetName::BHUNP_3BBB_ADVANCED:
      return QStringList{QString("2.20"),
                         QString("2.25"),
                         QString("2.30"),
                         QString("2.31"),
                         QString("2.35 to 2.42"),
                         QString("3.00 to 3.02"),
                         QString("3.03"),
                         QString("3.04 and 3.05")};
    case FeetName::CBBE_3BA_3BBB:
      return QStringList{QString("1.50"),
                         QString("1.51 to 1.55"),
                         QString("2.02 to 2.04"),
                         QString("2.06 - 2.11 - 2.12"),
                         QString("2.13 to 2.22 - 2.30 - 2.31")};
    case FeetName::COCO_BODY:
      if (aIsCBBEBody)
        return QStringList{QString("V4"), QString("V6"), QString("V6.8")};

      return QStringList{QString("V3"), QString("V4"), QString("V6"), QString("V6.8")};
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
    case FeetName::ASDASDF:
      return QStringList{QString("1.13")};
  }

  return QStringList();
}

QStringList DataLists::GetFeetVariantsList(const FeetName& aFeetName, const int aRelativeVersion, const bool aIsCBBEBody)
{
  switch (aFeetName)
  {
    case FeetName::_INVALID_VALUE:
    {
      return QStringList();
    }
    case FeetName::CBBE:
    {
      return QStringList{QString("CBBE Default Feet")};
    }
    case FeetName::BHUNP_3BBB_ADVANCED:
    {
      return QStringList{QString("BHUNP 3BBB Advanced Feet")};
    }
    case FeetName::CBBE_3BA_3BBB:
    {
      return QStringList{QString("CBBE 3BBB Feet")};
    }
    case FeetName::COCO_BODY:
    {
      // All the version numbers propose the variants below:
      QStringList lVariantsList{QString("COCO Feet - Normal")};

      // Only for "V6" and "V6.8"
      if ((aIsCBBEBody && (aRelativeVersion == 1 || aRelativeVersion == 2))      // CBBE
          || (!aIsCBBEBody && (aRelativeVersion == 2 || aRelativeVersion == 3))) // UUNP
      {
        lVariantsList.append(QString("COCO Feet - High Heels"));
      }

      return lVariantsList;
    }
    case FeetName::MIMIR_EBONIC_BODY:
    {
      return QStringList{QString("CBBE 3BBB Feet")};
    }
    case FeetName::MORE_SLIDERS_FOR_FEET:
    {
      return QStringList{QString("More Sliders for Feet - Normal"),
                         QString("More Sliders for Feet - High Heels")};
    }
    case FeetName::HG_FEET:
    {
      return QStringList{QString("HGFeet UUNP")};
    }
    case FeetName::KHRYSAMERE_HG_FEET:
    {
      return QStringList{QString("K Feet")};
    }
    case FeetName::ASDASDF:
    {
      return QStringList{QString("ASDASDF BHUNP Feet")};
    }
  }

  return QStringList();
}

QString DataLists::GetQRCResourceName(const BodyVariant& aBodyVariant, const FeetNameVersion& aFeetNameVersion)
{
  auto lFeetName{DataLists::GetName(aFeetNameVersion)};
  switch (lFeetName)
  {
    case FeetName::_INVALID_VALUE:
    {
      return QString();
    }
    case FeetName::CBBE:
    {
      return QString("cbbe");
    }
    case FeetName::BHUNP_3BBB_ADVANCED:
    {
      return QString("bhunp");
    }
    case FeetName::CBBE_3BA_3BBB:
    {
      return QString("cbbe 3bbb 3ba");
    }
    case FeetName::COCO_BODY:
    {
      const auto lFeetVariant{DataLists::GetVariant(aFeetNameVersion)};

      // CBBE
      if (Utils::IsCBBEBasedBody(aBodyVariant))
      {
        if (lFeetVariant == FeetVariant::COCO_BODY_HIGH_HEELS_CBBE)
        {
          return QString("coco body/high heels/cbbe");
        }

        return QString("coco body/normal/cbbe");
      }

      // UUNP
      if (lFeetVariant == FeetVariant::COCO_BODY_HIGH_HEELS_UUNP)
      {
        return QString("coco body/high heels/uunp");
      }

      return QString("coco body/normal/uunp");
    }
    case FeetName::MIMIR_EBONIC_BODY:
    {
      return QString("mimir ebonic body");
    }
    case FeetName::MORE_SLIDERS_FOR_FEET:
    {
      if (aFeetNameVersion == FeetNameVersion::MORE_SLIDERS_FOR_FEET_NORMAL_CBBE_1_0)
        return QString("msf/normal/cbbe");
      else if (aFeetNameVersion == FeetNameVersion::MORE_SLIDERS_FOR_FEET_NORMAL_BHUNP_1_1)
        return QString("msf/normal/bhunp");
      else if (aFeetNameVersion == FeetNameVersion::MORE_SLIDERS_FOR_FEET_HIGH_HEELS_CBBE_1_0)
        return QString("msf/high heels/cbbe");
      else if (aFeetNameVersion == FeetNameVersion::MORE_SLIDERS_FOR_FEET_HIGH_HEELS_BHUNP_1_1)
        return QString("msf/high heels/bhunp");

      break;
    }
    case FeetName::HG_FEET:
    {
      if (!Utils::IsCBBEBasedBody(aBodyVariant))
      {
        return QString("hg feet/bhunp");
      }

      break;
    }
    case FeetName::KHRYSAMERE_HG_FEET:
    {
      return QString("khrysamere hg feet");
    }
    case FeetName::ASDASDF:
    {
      return QString("asdasdf");
    }
  }

  return QString();
}

BodyName DataLists::GetName(const BodyVariant& aBodyVariant)
{
  // Return the parent body name of a given variant
  switch (aBodyVariant)
  {
    case BodyVariant::_INVALID_VALUE:
      return BodyName::_INVALID_VALUE;
    case BodyVariant::CBBE_3BA_3BBB:
    case BodyVariant::CBBE_3BA_3BBB_NEVERNUDE:
    case BodyVariant::CBBE_3BA_3BBB_UNDERWEAR:
      return BodyName::CBBE_3BA_3BBB;
    case BodyVariant::BHUNP_3BBB:
    case BodyVariant::BHUNP_3BBB_ADVANCED:
    case BodyVariant::BHUNP_3BBB_ADVANCED_VER_2:
    case BodyVariant::BHUNP_3BBB_ADVANCED_VER_3:
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
    case BodyVariant::ASDASDF_3BBB:
    case BodyVariant::ASDASDF_3BBB_NO_THIGH_JIGGLE:
    case BodyVariant::ASDASDF_BBP:
    case BodyVariant::ASDASDF_BBP_NO_THIGH_JIGGLE:
    case BodyVariant::ASDASDF_TBBP:
    case BodyVariant::ASDASDF_TBBP_NO_THIGH_JIGGLE:
    case BodyVariant::ASDASDF_ALTERBODY_3BBB:
    case BodyVariant::ASDASDF_ALTERBODY_3BBB_NO_THIGH_JIGGLE:
    case BodyVariant::ASDASDF_ALTERBODY_BBP:
    case BodyVariant::ASDASDF_ALTERBODY_BBP_NO_THIGH_JIGGLE:
    case BodyVariant::ASDASDF_ALTERBODY_TBBP:
    case BodyVariant::ASDASDF_ALTERBODY_TBBP_NO_THIGH_JIGGLE:
      return BodyName::BHUNP_LITE_ASDASDF;
  }

  return BodyName::_INVALID_VALUE;
}

FeetName DataLists::GetName(const FeetVariant& aFeetVariant)
{
  switch (aFeetVariant)
  {
    case FeetVariant::_INVALID_VALUE:
      return FeetName::_INVALID_VALUE;
    case FeetVariant::CBBE:
      return FeetName::CBBE;
    case FeetVariant::BHUNP_3BBB_ADVANCED:
      return FeetName::BHUNP_3BBB_ADVANCED;
    case FeetVariant::CBBE_3BA_3BBB:
      return FeetName::CBBE_3BA_3BBB;
    case FeetVariant::COCO_BODY_NORMAL_CBBE:
    case FeetVariant::COCO_BODY_NORMAL_UUNP:
    case FeetVariant::COCO_BODY_HIGH_HEELS_CBBE:
    case FeetVariant::COCO_BODY_HIGH_HEELS_UUNP:
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
    case FeetVariant::ASDASDF:
      return FeetName::ASDASDF;
  }

  return FeetName::_INVALID_VALUE;
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
    case BodyVariant::_INVALID_VALUE:
    {
      return FeetName::_INVALID_VALUE;
    }
    case BodyVariant::CBBE_3BA_3BBB:
    case BodyVariant::CBBE_3BA_3BBB_NEVERNUDE:
    case BodyVariant::CBBE_3BA_3BBB_UNDERWEAR:
    {
      switch (aRelativeFeetName)
      {
        case 0:
          return FeetName::CBBE_3BA_3BBB;
        case 1:
          return FeetName::MORE_SLIDERS_FOR_FEET;
      }

      return FeetName::_INVALID_VALUE;
    }
    case BodyVariant::BHUNP_3BBB:
    case BodyVariant::BHUNP_3BBB_ADVANCED:
    case BodyVariant::BHUNP_3BBB_ADVANCED_VER_2:
    case BodyVariant::BHUNP_3BBB_ADVANCED_VER_3:
    case BodyVariant::BHUNP_BBP:
    case BodyVariant::BHUNP_BBP_ADVANCED:
    case BodyVariant::BHUNP_TBBP:
    case BodyVariant::BHUNP_TBBP_ADVANCED:
    case BodyVariant::BHUNP_3BBB_ADVANCED_VER_2_NEVERNUDE:
    {
      switch (aRelativeFeetName)
      {
        case 0:
          return FeetName::BHUNP_3BBB_ADVANCED;
        case 1:
          return FeetName::MORE_SLIDERS_FOR_FEET;
        case 2:
          return FeetName::HG_FEET;
        case 3:
          return FeetName::KHRYSAMERE_HG_FEET;
      }

      return FeetName::_INVALID_VALUE;
    }
    case BodyVariant::CBBE_SMP_3BBB:
    {
      switch (aRelativeFeetName)
      {
        case 0:
          return FeetName::CBBE;
        case 1:
          return FeetName::MORE_SLIDERS_FOR_FEET;
      }

      return FeetName::_INVALID_VALUE;
    }
    case BodyVariant::COCO_BODY_CBBE:
    {
      switch (aRelativeFeetName)
      {
        case 0:
          return FeetName::COCO_BODY;
        case 1:
          return FeetName::MORE_SLIDERS_FOR_FEET;
      }

      return FeetName::_INVALID_VALUE;
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
      }

      return FeetName::_INVALID_VALUE;
    }
    case BodyVariant::MIMIR_EBONIC_BODY:
    {
      switch (aRelativeFeetName)
      {
        case 0:
          return FeetName::MIMIR_EBONIC_BODY;
        case 1:
          return FeetName::MORE_SLIDERS_FOR_FEET;
      }

      return FeetName::_INVALID_VALUE;
    }
    case BodyVariant::ASDASDF_3BBB:
    case BodyVariant::ASDASDF_3BBB_NO_THIGH_JIGGLE:
    case BodyVariant::ASDASDF_BBP:
    case BodyVariant::ASDASDF_BBP_NO_THIGH_JIGGLE:
    case BodyVariant::ASDASDF_TBBP:
    case BodyVariant::ASDASDF_TBBP_NO_THIGH_JIGGLE:
    case BodyVariant::ASDASDF_ALTERBODY_3BBB:
    case BodyVariant::ASDASDF_ALTERBODY_3BBB_NO_THIGH_JIGGLE:
    case BodyVariant::ASDASDF_ALTERBODY_BBP:
    case BodyVariant::ASDASDF_ALTERBODY_BBP_NO_THIGH_JIGGLE:
    case BodyVariant::ASDASDF_ALTERBODY_TBBP:
    case BodyVariant::ASDASDF_ALTERBODY_TBBP_NO_THIGH_JIGGLE:
    {
      switch (aRelativeFeetName)
      {
        case 0:
          return FeetName::ASDASDF;
        case 1:
          return FeetName::MORE_SLIDERS_FOR_FEET;
        case 2:
          return FeetName::HG_FEET;
        case 3:
          return FeetName::KHRYSAMERE_HG_FEET;
      }

      return FeetName::_INVALID_VALUE;
    }
  }

  return FeetName::_INVALID_VALUE;
}

BodyVariant DataLists::GetVariant(const BodyNameVersion& aBodyNameVersion)
{
  switch (aBodyNameVersion)
  {
    //
    case BodyNameVersion::_INVALID_VALUE:
      return BodyVariant::_INVALID_VALUE;
    //
    case BodyNameVersion::CBBE_3BA_3BBB_1_50:
    case BodyNameVersion::CBBE_3BA_3BBB_1_51_TO_1_55:
    case BodyNameVersion::CBBE_3BA_3BBB_2_02_TO_2_04:
    case BodyNameVersion::CBBE_3BA_3BBB_2_06_AND_2_11_AND_2_12:
    case BodyNameVersion::CBBE_3BA_3BBB_2_13_TO_2_18_AND_2_30_AND_2_31:
      return BodyVariant::CBBE_3BA_3BBB;
    //
    case BodyNameVersion::CBBE_3BA_3BBB_NEVERNUDE_2_13_TO_2_18_AND_2_30_AND_2_31:
      return BodyVariant::CBBE_3BA_3BBB_NEVERNUDE;
    //
    case BodyNameVersion::CBBE_3BA_3BBB_UNDERWEAR_2_13_TO_2_18_AND_2_30_AND_2_31:
      return BodyVariant::CBBE_3BA_3BBB_UNDERWEAR;
    //
    case BodyNameVersion::BHUNP_3BBB_2_20:
    case BodyNameVersion::BHUNP_3BBB_2_25:
    case BodyNameVersion::BHUNP_3BBB_2_30:
    case BodyNameVersion::BHUNP_3BBB_2_31:
    case BodyNameVersion::BHUNP_3BBB_2_35_TO_2_42:
    case BodyNameVersion::BHUNP_3BBB_3_00_TO_3_02:
    case BodyNameVersion::BHUNP_3BBB_3_03:
    case BodyNameVersion::BHUNP_3BBB_3_04:
      return BodyVariant::BHUNP_3BBB;
    //
    case BodyNameVersion::BHUNP_3BBB_ADVANCED_2_20:
    case BodyNameVersion::BHUNP_3BBB_ADVANCED_2_25:
    case BodyNameVersion::BHUNP_3BBB_ADVANCED_2_30:
    case BodyNameVersion::BHUNP_3BBB_ADVANCED_2_31:
    case BodyNameVersion::BHUNP_3BBB_ADVANCED_2_35_TO_2_42:
    case BodyNameVersion::BHUNP_3BBB_ADVANCED_3_00_TO_3_02:
    case BodyNameVersion::BHUNP_3BBB_ADVANCED_3_03:
    case BodyNameVersion::BHUNP_3BBB_ADVANCED_3_04:
      return BodyVariant::BHUNP_3BBB_ADVANCED;
      //
    case BodyNameVersion::BHUNP_3BBB_ADVANCED_VER_2_2_20:
    case BodyNameVersion::BHUNP_3BBB_ADVANCED_VER_2_2_25:
    case BodyNameVersion::BHUNP_3BBB_ADVANCED_VER_2_2_30:
    case BodyNameVersion::BHUNP_3BBB_ADVANCED_VER_2_2_31:
    case BodyNameVersion::BHUNP_3BBB_ADVANCED_VER_2_2_35_TO_2_42:
    case BodyNameVersion::BHUNP_3BBB_ADVANCED_VER_2_3_00_TO_3_02:
    case BodyNameVersion::BHUNP_3BBB_ADVANCED_VER_2_3_03:
    case BodyNameVersion::BHUNP_3BBB_ADVANCED_VER_2_3_04:
      return BodyVariant::BHUNP_3BBB_ADVANCED_VER_2;
      //
    case BodyNameVersion::BHUNP_3BBB_ADVANCED_VER_3_3_00_TO_3_02:
    case BodyNameVersion::BHUNP_3BBB_ADVANCED_VER_3_3_03:
    case BodyNameVersion::BHUNP_3BBB_ADVANCED_VER_3_3_04:
      return BodyVariant::BHUNP_3BBB_ADVANCED_VER_3;
    //
    case BodyNameVersion::BHUNP_BBP_2_20:
    case BodyNameVersion::BHUNP_BBP_2_25:
    case BodyNameVersion::BHUNP_BBP_2_30:
    case BodyNameVersion::BHUNP_BBP_2_31:
    case BodyNameVersion::BHUNP_BBP_2_35_TO_2_42:
    case BodyNameVersion::BHUNP_BBP_3_00_TO_3_02:
    case BodyNameVersion::BHUNP_BBP_3_03:
    case BodyNameVersion::BHUNP_BBP_3_04:
      return BodyVariant::BHUNP_BBP;
    //
    case BodyNameVersion::BHUNP_BBP_ADVANCED_2_20:
    case BodyNameVersion::BHUNP_BBP_ADVANCED_2_25:
    case BodyNameVersion::BHUNP_BBP_ADVANCED_2_30:
    case BodyNameVersion::BHUNP_BBP_ADVANCED_2_31:
    case BodyNameVersion::BHUNP_BBP_ADVANCED_2_35_TO_2_42:
    case BodyNameVersion::BHUNP_BBP_ADVANCED_3_00_TO_3_02:
    case BodyNameVersion::BHUNP_BBP_ADVANCED_3_03:
    case BodyNameVersion::BHUNP_BBP_ADVANCED_3_04:
      return BodyVariant::BHUNP_BBP_ADVANCED;
    //
    case BodyNameVersion::BHUNP_TBBP_2_20:
    case BodyNameVersion::BHUNP_TBBP_2_25:
    case BodyNameVersion::BHUNP_TBBP_2_30:
    case BodyNameVersion::BHUNP_TBBP_2_31:
    case BodyNameVersion::BHUNP_TBBP_2_35_TO_2_42:
    case BodyNameVersion::BHUNP_TBBP_3_00_TO_3_02:
    case BodyNameVersion::BHUNP_TBBP_3_03:
    case BodyNameVersion::BHUNP_TBBP_3_04:
      return BodyVariant::BHUNP_TBBP;
    //
    case BodyNameVersion::BHUNP_TBBP_ADVANCED_2_20:
    case BodyNameVersion::BHUNP_TBBP_ADVANCED_2_25:
    case BodyNameVersion::BHUNP_TBBP_ADVANCED_2_30:
    case BodyNameVersion::BHUNP_TBBP_ADVANCED_2_31:
    case BodyNameVersion::BHUNP_TBBP_ADVANCED_2_35_TO_2_42:
    case BodyNameVersion::BHUNP_TBBP_ADVANCED_3_00_TO_3_02:
    case BodyNameVersion::BHUNP_TBBP_ADVANCED_3_03:
    case BodyNameVersion::BHUNP_TBBP_ADVANCED_3_04:
      return BodyVariant::BHUNP_TBBP_ADVANCED;
    //
    case BodyNameVersion::BHUNP_3BBB_ADVANCED_VER_2_NEVERNUDE_2_25:
    case BodyNameVersion::BHUNP_3BBB_ADVANCED_VER_2_NEVERNUDE_2_30:
    case BodyNameVersion::BHUNP_3BBB_ADVANCED_VER_2_NEVERNUDE_2_35_TO_2_42:
    case BodyNameVersion::BHUNP_3BBB_ADVANCED_VER_2_NEVERNUDE_3_00_TO_3_02:
    case BodyNameVersion::BHUNP_3BBB_ADVANCED_VER_2_NEVERNUDE_3_03:
    case BodyNameVersion::BHUNP_3BBB_ADVANCED_VER_2_NEVERNUDE_3_04:
      return BodyVariant::BHUNP_3BBB_ADVANCED_VER_2_NEVERNUDE;
    //
    case BodyNameVersion::CBBE_SMP_3BBB_1_2_0:
      return BodyVariant::CBBE_SMP_3BBB;
    //
    case BodyNameVersion::COCO_BODY_CBBE_V4:
    case BodyNameVersion::COCO_BODY_CBBE_V6:
    case BodyNameVersion::COCO_BODY_CBBE_V6_8:
      return BodyVariant::COCO_BODY_CBBE;
    //
    case BodyNameVersion::COCO_BODY_UUNP_V3:
    case BodyNameVersion::COCO_BODY_UUNP_V4:
    case BodyNameVersion::COCO_BODY_UUNP_V6:
    case BodyNameVersion::COCO_BODY_UUNP_V6_8:
      return BodyVariant::COCO_BODY_UUNP;
    //
    case BodyNameVersion::MIMIR_EBONIC_BODY_1_2:
    case BodyNameVersion::MIMIR_EBONIC_BODY_1_2_FOOT_SEAMS_FIX:
      return BodyVariant::MIMIR_EBONIC_BODY;
    //
    case BodyNameVersion::ASDASDF_3BBB_1_13:
      return BodyVariant::ASDASDF_3BBB;
    //
    case BodyNameVersion::ASDASDF_3BBB_NO_THIGH_JIGGLE_1_13:
      return BodyVariant::ASDASDF_3BBB_NO_THIGH_JIGGLE;
    //
    case BodyNameVersion::ASDASDF_BBP_1_13:
      return BodyVariant::ASDASDF_BBP;
    //
    case BodyNameVersion::ASDASDF_BBP_NO_THIGH_JIGGLE_1_13:
      return BodyVariant::ASDASDF_BBP_NO_THIGH_JIGGLE;
    //
    case BodyNameVersion::ASDASDF_TBBP_1_13:
      return BodyVariant::ASDASDF_TBBP;
    //
    case BodyNameVersion::ASDASDF_TBBP_NO_THIGH_JIGGLE_1_13:
      return BodyVariant::ASDASDF_TBBP_NO_THIGH_JIGGLE;
    //
    case BodyNameVersion::ASDASDF_ALTERBODY_3BBB_1_13:
      return BodyVariant::ASDASDF_ALTERBODY_3BBB;
    //
    case BodyNameVersion::ASDASDF_ALTERBODY_3BBB_NO_THIGH_JIGGLE_1_13:
      return BodyVariant::ASDASDF_ALTERBODY_3BBB_NO_THIGH_JIGGLE;
    //
    case BodyNameVersion::ASDASDF_ALTERBODY_BBP_1_13:
      return BodyVariant::ASDASDF_ALTERBODY_BBP;
    //
    case BodyNameVersion::ASDASDF_ALTERBODY_BBP_NO_THIGH_JIGGLE_1_13:
      return BodyVariant::ASDASDF_ALTERBODY_BBP_NO_THIGH_JIGGLE;
    //
    case BodyNameVersion::ASDASDF_ALTERBODY_TBBP_1_13:
      return BodyVariant::ASDASDF_ALTERBODY_TBBP;
    //
    case BodyNameVersion::ASDASDF_ALTERBODY_TBBP_NO_THIGH_JIGGLE_1_13:
      return BodyVariant::ASDASDF_ALTERBODY_TBBP_NO_THIGH_JIGGLE;
  }

  return BodyVariant::_INVALID_VALUE;
}

BodyVariant DataLists::GetVariant(const BodyName& aBodyName, const int aRelativeVersion, const int aRelativeVariant)
{
  switch (aBodyName)
  {
    case BodyName::_INVALID_VALUE:
      return BodyVariant::_INVALID_VALUE;
    case BodyName::CBBE_3BA_3BBB:
    {
      // If not "2.13 to 2.22 - 2.30 - 2.31" but trying to use nevernude or underwear key, this is invalid
      if (aRelativeVersion != 4 && aRelativeVariant > 0)
      {
        return BodyVariant::_INVALID_VALUE;
      }

      return static_cast<BodyVariant>(aRelativeVariant + static_cast<int>(BodyVariant::CBBE_3BA_3BBB));
    }
    case BodyName::BHUNP_UUNP_NEXT_GENERATION:
    {
      auto lOffset{0};

      switch (aRelativeVersion)
      {
        case 0: // "2.20"
        case 3: // "2.31"
        {
          if (aRelativeVariant > 6) // Cannot use "BodyVariant::BHUNP_3BBB_ADVANCED_VER_2_NEVERNUDE"
          {
            return BodyVariant::_INVALID_VALUE;
          }

          lOffset = aRelativeVariant >= 3 ? 1 : 0; // Skip "BodyVariant::BHUNP_3BBB_ADVANCED_VER_3"
          break;
        }
        case 1: // "2.25"
        case 2: // "2.30"
        case 4: // "2.35 to 2.42"
        {
          lOffset = aRelativeVariant >= 3 ? 1 : 0; // Skip "BodyVariant::BHUNP_3BBB_ADVANCED_VER_3"
          break;
        }
        case 5: // "3.00 to 3.02"
        case 6: // "3.03"
        case 7: // "3.04 and 3.05"
        {
          break;
        }
      }

      // BodyVariant::BHUNP_3BBB is the first variant
      return static_cast<BodyVariant>(aRelativeVariant + lOffset + static_cast<int>(BodyVariant::BHUNP_3BBB));
    }
    case BodyName::CBBE_SMP_3BBB:
      return BodyVariant::CBBE_SMP_3BBB;
    case BodyName::COCO_BODY_CBBE:
      return BodyVariant::COCO_BODY_CBBE;
    case BodyName::COCO_BODY_UUNP:
      return BodyVariant::COCO_BODY_UUNP;
    case BodyName::MIMIR_EBONIC_BODY:
      return BodyVariant::MIMIR_EBONIC_BODY;
    case BodyName::BHUNP_LITE_ASDASDF:
      // BodyVariant::ASDASDF_3BBB is the first variant
      return static_cast<BodyVariant>(aRelativeVariant + static_cast<int>(BodyVariant::ASDASDF_3BBB));
  }

  return BodyVariant::_INVALID_VALUE;
}

FeetVariant DataLists::GetVariant(const FeetNameVersion& aFeetNameVersion)
{
  switch (aFeetNameVersion)
  {
    case FeetNameVersion::_INVALID_VALUE:
      return FeetVariant::_INVALID_VALUE;
    case FeetNameVersion::CBBE_1_6_1:
      return FeetVariant::CBBE;
    case FeetNameVersion::BHUNP_3BBB_ADVANCED_2_20:
    case FeetNameVersion::BHUNP_3BBB_ADVANCED_2_25:
    case FeetNameVersion::BHUNP_3BBB_ADVANCED_2_30:
    case FeetNameVersion::BHUNP_3BBB_ADVANCED_2_31:
    case FeetNameVersion::BHUNP_3BBB_ADVANCED_2_35_TO_2_42:
    case FeetNameVersion::BHUNP_3BBB_ADVANCED_3_00_TO_3_02:
    case FeetNameVersion::BHUNP_3BBB_ADVANCED_3_03:
    case FeetNameVersion::BHUNP_3BBB_ADVANCED_3_04:
      return FeetVariant::BHUNP_3BBB_ADVANCED;
    case FeetNameVersion::CBBE_3BA_3BBB_1_50:
    case FeetNameVersion::CBBE_3BA_3BBB_1_51_TO_1_55:
    case FeetNameVersion::CBBE_3BA_3BBB_2_02_TO_2_04:
    case FeetNameVersion::CBBE_3BA_3BBB_2_06_AND_2_11_AND_2_12:
    case FeetNameVersion::CBBE_3BA_3BBB_2_13_TO_2_18_AND_2_30_AND_2_31:
      return FeetVariant::CBBE_3BA_3BBB;
    case FeetNameVersion::COCO_BODY_NORMAL_CBBE_V4:
    case FeetNameVersion::COCO_BODY_NORMAL_CBBE_V6:
    case FeetNameVersion::COCO_BODY_NORMAL_CBBE_V6_8:
      return FeetVariant::COCO_BODY_NORMAL_CBBE;
    case FeetNameVersion::COCO_BODY_NORMAL_UUNP_V3:
    case FeetNameVersion::COCO_BODY_NORMAL_UUNP_V4:
    case FeetNameVersion::COCO_BODY_NORMAL_UUNP_V6:
    case FeetNameVersion::COCO_BODY_NORMAL_UUNP_V6_8:
      return FeetVariant::COCO_BODY_NORMAL_UUNP;
    case FeetNameVersion::COCO_BODY_HIGH_HEELS_CBBE_V6:
    case FeetNameVersion::COCO_BODY_HIGH_HEELS_CBBE_V6_8:
      return FeetVariant::COCO_BODY_HIGH_HEELS_CBBE;
    case FeetNameVersion::COCO_BODY_HIGH_HEELS_UUNP_V6:
    case FeetNameVersion::COCO_BODY_HIGH_HEELS_UUNP_V6_8:
      return FeetVariant::COCO_BODY_HIGH_HEELS_UUNP;
    case FeetNameVersion::MIMIR_EBONIC_BODY_1_2:
    case FeetNameVersion::MIMIR_EBONIC_BODY_1_2_FOOT_SEAMS_FIX:
      return FeetVariant::MIMIR_EBONIC_BODY;
    case FeetNameVersion::ASDASDF_1_13:
      return FeetVariant::ASDASDF;
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
  }

  return FeetVariant::_INVALID_VALUE;
}

FeetVariant DataLists::GetVariant(const FeetName& aFeetName, const int aRelativeVersion, const int aRelativeVariant, const bool aIsCBBEBody)
{
  Q_UNUSED(aRelativeVersion);

  switch (aFeetName)
  {
    case FeetName::_INVALID_VALUE:
    {
      return FeetVariant::_INVALID_VALUE;
    }
    case FeetName::CBBE:
    {
      return FeetVariant::CBBE;
    }
    case FeetName::BHUNP_3BBB_ADVANCED:
    {
      return FeetVariant::BHUNP_3BBB_ADVANCED;
    }
    case FeetName::CBBE_3BA_3BBB:
    {
      return FeetVariant::CBBE_3BA_3BBB;
    }
    case FeetName::COCO_BODY:
    {
      // CBBE
      if (aIsCBBEBody)
      {
        if (aRelativeVariant == 0)
        {
          return FeetVariant::COCO_BODY_NORMAL_CBBE;
        }
        return FeetVariant::COCO_BODY_HIGH_HEELS_CBBE;
      }

      // UNP
      if (aRelativeVariant == 0)
      {
        return FeetVariant::COCO_BODY_NORMAL_UUNP;
      }

      return FeetVariant::COCO_BODY_HIGH_HEELS_UUNP;
    }
    case FeetName::MIMIR_EBONIC_BODY:
    {
      return FeetVariant::MIMIR_EBONIC_BODY;
    }
    case FeetName::ASDASDF:
    {
      return FeetVariant::ASDASDF;
    }
    case FeetName::MORE_SLIDERS_FOR_FEET:
    {
      // CBBE
      if (aIsCBBEBody)
      {
        if (aRelativeVariant == 0)
        {
          return FeetVariant::MORE_SLIDERS_FOR_FEET_NORMAL_CBBE;
        }
        return FeetVariant::MORE_SLIDERS_FOR_FEET_HIGH_HEELS_CBBE;
      }

      // UNP
      if (aRelativeVariant == 0)
      {
        return FeetVariant::MORE_SLIDERS_FOR_FEET_NORMAL_BHUNP;
      }

      return FeetVariant::MORE_SLIDERS_FOR_FEET_HIGH_HEELS_BHUNP;
    }
    case FeetName::HG_FEET:
    {
      return FeetVariant::HG_FEET;
    }
    case FeetName::KHRYSAMERE_HG_FEET:
    {
      return FeetVariant::KHRYSAMERE_HG_FEET;
    }
  }

  return FeetVariant::_INVALID_VALUE;
}

BodyNameVersion DataLists::GetBodyNameVersion(const BodyVariant& aBodyVariant, const int aRelativeVersion)
{
  // Ignore the cases where the checkboxes have no index selected
  if (aRelativeVersion == -1)
    return BodyNameVersion::_INVALID_VALUE;

  // Avoid having an undefined version selected
  if (aRelativeVersion >= DataLists::GetBodyVersionsList(DataLists::GetName(aBodyVariant)).size())
    return BodyNameVersion::_INVALID_VALUE;

  if (!Utils::IsVersionOffsetValid(aBodyVariant, aRelativeVersion))
  {
    return BodyNameVersion::_INVALID_VALUE;
  }

  const auto lOffsetFirstKey{static_cast<int>(DataLists::GetFirstKey(aBodyVariant)) - DataLists::GetVersionOffset(aBodyVariant, aRelativeVersion)};
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

  if (!Utils::IsVersionOffsetValid(aFeetVariant, aRelativeVersion))
  {
    return FeetNameVersion::_INVALID_VALUE;
  }

  const auto lFeetName{DataLists::GetName(aFeetVariant)};
  const auto lOffsetFirstKey{static_cast<int>(DataLists::GetFirstKey(aFeetVariant)) - DataLists::GetVersionOffset(aFeetVariant)};

  switch (lFeetName)
  {
    case FeetName::_INVALID_VALUE:
      return FeetNameVersion::_INVALID_VALUE;
    case FeetName::CBBE:
    case FeetName::BHUNP_3BBB_ADVANCED:
    case FeetName::CBBE_3BA_3BBB:
    case FeetName::COCO_BODY:
    case FeetName::MIMIR_EBONIC_BODY:
    case FeetName::ASDASDF:
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
    return -1;

  const auto lBodyVariant{DataLists::GetVariant(aBodyNameVersion)};
  const auto lOffsetFirstKey{static_cast<int>(DataLists::GetFirstKey(lBodyVariant))};
  return (static_cast<int>(aBodyNameVersion) - lOffsetFirstKey + DataLists::GetVersionOffset(aBodyNameVersion));
}

int DataLists::GetVariantIndex(const BodyNameVersion& aBodyNameVersion)
{
  if (aBodyNameVersion == BodyNameVersion::_INVALID_VALUE)
    return -1;

  const auto lBodyVariant{DataLists::GetVariant(aBodyNameVersion)};
  switch (lBodyVariant)
  {
    case BodyVariant::_INVALID_VALUE:
      return -1;
    case BodyVariant::CBBE_3BA_3BBB:
    case BodyVariant::CBBE_SMP_3BBB:
    case BodyVariant::COCO_BODY_CBBE:
    case BodyVariant::COCO_BODY_UUNP:
    case BodyVariant::MIMIR_EBONIC_BODY:
      return 0;
    case BodyVariant::CBBE_3BA_3BBB_NEVERNUDE:
      return 1;
    case BodyVariant::CBBE_3BA_3BBB_UNDERWEAR:
      return 2;
    case BodyVariant::BHUNP_3BBB:
    case BodyVariant::BHUNP_3BBB_ADVANCED:
    case BodyVariant::BHUNP_3BBB_ADVANCED_VER_2:
    case BodyVariant::BHUNP_3BBB_ADVANCED_VER_3:
    case BodyVariant::BHUNP_BBP:
    case BodyVariant::BHUNP_BBP_ADVANCED:
    case BodyVariant::BHUNP_TBBP:
    case BodyVariant::BHUNP_TBBP_ADVANCED:
    case BodyVariant::BHUNP_3BBB_ADVANCED_VER_2_NEVERNUDE:
    {
      const auto lVersionIndex{DataLists::GetVersionIndex(aBodyNameVersion)};
      auto lOffset{0};
      switch (lVersionIndex)
      {
        case 0: // "2.20"
        case 1: // "2.25"
        case 2: // "2.30"
        case 3: // "2.31"
        case 4: // "2.35 to 2.42"
        {
          switch (lBodyVariant)
          {
            case BodyVariant::BHUNP_3BBB:
            case BodyVariant::BHUNP_3BBB_ADVANCED:
            case BodyVariant::BHUNP_3BBB_ADVANCED_VER_2:
            {
              lOffset = 0;
              break;
            }
            case BodyVariant::BHUNP_BBP:
            case BodyVariant::BHUNP_BBP_ADVANCED:
            case BodyVariant::BHUNP_TBBP:
            case BodyVariant::BHUNP_TBBP_ADVANCED:
            case BodyVariant::BHUNP_3BBB_ADVANCED_VER_2_NEVERNUDE:
            {
              // Skip "BodyVariant::BHUNP_3BBB_ADVANCED_VER_3-"
              lOffset = 1;
              break;
            }
            default:
              break;
          }

          break;
        }
        case 5: // "3.00 to 3.02"
        case 6: // "3.03"
        case 7: // "3.04 and 3.05"
        {
          break;
        }
      }

      return static_cast<int>(lBodyVariant) - lOffset - static_cast<int>(BodyVariant::BHUNP_3BBB);
    }
    case BodyVariant::ASDASDF_3BBB:
    case BodyVariant::ASDASDF_3BBB_NO_THIGH_JIGGLE:
    case BodyVariant::ASDASDF_BBP:
    case BodyVariant::ASDASDF_BBP_NO_THIGH_JIGGLE:
    case BodyVariant::ASDASDF_TBBP:
    case BodyVariant::ASDASDF_TBBP_NO_THIGH_JIGGLE:
    case BodyVariant::ASDASDF_ALTERBODY_3BBB:
    case BodyVariant::ASDASDF_ALTERBODY_3BBB_NO_THIGH_JIGGLE:
    case BodyVariant::ASDASDF_ALTERBODY_BBP:
    case BodyVariant::ASDASDF_ALTERBODY_BBP_NO_THIGH_JIGGLE:
    case BodyVariant::ASDASDF_ALTERBODY_TBBP:
    case BodyVariant::ASDASDF_ALTERBODY_TBBP_NO_THIGH_JIGGLE:
      return static_cast<int>(lBodyVariant) - static_cast<int>(BodyVariant::ASDASDF_3BBB);
  }

  return -1;
}

int DataLists::GetNameIndex(const BodyVariant& aBodyVariant, const FeetNameVersion& aFeetNameVersion)
{
  const auto lFeetName{DataLists::GetName(aFeetNameVersion)};

  switch (aBodyVariant)
  {
    case BodyVariant::_INVALID_VALUE:
    {
      return -1;
    }
    case BodyVariant::CBBE_3BA_3BBB:
    case BodyVariant::CBBE_3BA_3BBB_NEVERNUDE:
    case BodyVariant::CBBE_3BA_3BBB_UNDERWEAR:
    {
      switch (lFeetName)
      {
        case FeetName::CBBE_3BA_3BBB:
          return 0;
        case FeetName::MORE_SLIDERS_FOR_FEET:
          return 1;
        default:
          // Prevent warnings:
          return -1;
      }
    }
    case BodyVariant::BHUNP_3BBB:
    case BodyVariant::BHUNP_3BBB_ADVANCED:
    case BodyVariant::BHUNP_3BBB_ADVANCED_VER_2:
    case BodyVariant::BHUNP_3BBB_ADVANCED_VER_3:
    case BodyVariant::BHUNP_BBP:
    case BodyVariant::BHUNP_BBP_ADVANCED:
    case BodyVariant::BHUNP_TBBP:
    case BodyVariant::BHUNP_TBBP_ADVANCED:
    case BodyVariant::BHUNP_3BBB_ADVANCED_VER_2_NEVERNUDE:
    {
      switch (lFeetName)
      {
        case FeetName::BHUNP_3BBB_ADVANCED:
          return 0;
        case FeetName::MORE_SLIDERS_FOR_FEET:
          return 1;
        case FeetName::HG_FEET:
          return 2;
        case FeetName::KHRYSAMERE_HG_FEET:
          return 3;
        default:
          // Prevent warnings:
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
          // Prevent warnings:
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
          // Prevent warnings:
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
          // Prevent warnings:
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
          // Prevent warnings:
          return -1;
      }
    }
    case BodyVariant::ASDASDF_3BBB:
    case BodyVariant::ASDASDF_3BBB_NO_THIGH_JIGGLE:
    case BodyVariant::ASDASDF_BBP:
    case BodyVariant::ASDASDF_BBP_NO_THIGH_JIGGLE:
    case BodyVariant::ASDASDF_TBBP:
    case BodyVariant::ASDASDF_TBBP_NO_THIGH_JIGGLE:
    case BodyVariant::ASDASDF_ALTERBODY_3BBB:
    case BodyVariant::ASDASDF_ALTERBODY_3BBB_NO_THIGH_JIGGLE:
    case BodyVariant::ASDASDF_ALTERBODY_BBP:
    case BodyVariant::ASDASDF_ALTERBODY_BBP_NO_THIGH_JIGGLE:
    case BodyVariant::ASDASDF_ALTERBODY_TBBP:
    case BodyVariant::ASDASDF_ALTERBODY_TBBP_NO_THIGH_JIGGLE:
    {
      switch (lFeetName)
      {
        case FeetName::ASDASDF:
          return 0;
        case FeetName::MORE_SLIDERS_FOR_FEET:
          return 1;
        case FeetName::HG_FEET:
          return 2;
        case FeetName::KHRYSAMERE_HG_FEET:
          return 3;
        default:
          // Prevent warnings:
          return -1;
      }
    }
  }

  return -1;
}

int DataLists::GetVersionIndex(const FeetNameVersion& aFeetNameVersion)
{
  if (aFeetNameVersion == FeetNameVersion::_INVALID_VALUE)
  {
    return -1;
  }

  const auto lFeetVariant{DataLists::GetVariant(aFeetNameVersion)};
  const auto lOffsetFirstKey{static_cast<int>(DataLists::GetFirstKey(lFeetVariant)) - DataLists::GetVersionOffset(lFeetVariant)};
  return (static_cast<int>(aFeetNameVersion) - lOffsetFirstKey);
}

int DataLists::GetVariantIndex(const FeetNameVersion& aFeetNameVersion)
{
  if (aFeetNameVersion == FeetNameVersion::_INVALID_VALUE)
    return -1;

  const auto lFeetVariant{DataLists::GetVariant(aFeetNameVersion)};
  switch (lFeetVariant)
  {
    case FeetVariant::_INVALID_VALUE:
      return -1;
    case FeetVariant::CBBE:
    case FeetVariant::BHUNP_3BBB_ADVANCED:
    case FeetVariant::CBBE_3BA_3BBB:
    case FeetVariant::COCO_BODY_NORMAL_CBBE:
    case FeetVariant::COCO_BODY_NORMAL_UUNP:
    case FeetVariant::MIMIR_EBONIC_BODY:
    case FeetVariant::ASDASDF:
    case FeetVariant::MORE_SLIDERS_FOR_FEET_NORMAL_CBBE:
    case FeetVariant::MORE_SLIDERS_FOR_FEET_NORMAL_BHUNP:
    case FeetVariant::HG_FEET:
    case FeetVariant::KHRYSAMERE_HG_FEET:
      return 0;
    case FeetVariant::COCO_BODY_HIGH_HEELS_CBBE:
    case FeetVariant::COCO_BODY_HIGH_HEELS_UUNP:
    case FeetVariant::MORE_SLIDERS_FOR_FEET_HIGH_HEELS_CBBE:
    case FeetVariant::MORE_SLIDERS_FOR_FEET_HIGH_HEELS_BHUNP:
      return 1;
  }

  return -1;
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
    case BodyVariant::_INVALID_VALUE:
      return BodyNameVersion::_INVALID_VALUE;
    case BodyVariant::CBBE_3BA_3BBB:
      return BodyNameVersion::CBBE_3BA_3BBB_1_50;
    case BodyVariant::CBBE_3BA_3BBB_NEVERNUDE:
      return BodyNameVersion::CBBE_3BA_3BBB_NEVERNUDE_2_13_TO_2_18_AND_2_30_AND_2_31;
    case BodyVariant::CBBE_3BA_3BBB_UNDERWEAR:
      return BodyNameVersion::CBBE_3BA_3BBB_UNDERWEAR_2_13_TO_2_18_AND_2_30_AND_2_31;
    case BodyVariant::BHUNP_3BBB:
      return BodyNameVersion::BHUNP_3BBB_2_20;
    case BodyVariant::BHUNP_3BBB_ADVANCED:
      return BodyNameVersion::BHUNP_3BBB_ADVANCED_2_20;
    case BodyVariant::BHUNP_3BBB_ADVANCED_VER_2:
      return BodyNameVersion::BHUNP_3BBB_ADVANCED_VER_2_2_20;
    case BodyVariant::BHUNP_3BBB_ADVANCED_VER_3:
      return BodyNameVersion::BHUNP_3BBB_ADVANCED_VER_3_3_00_TO_3_02;
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
    case BodyVariant::ASDASDF_3BBB:
      return BodyNameVersion::ASDASDF_3BBB_1_13;
    case BodyVariant::ASDASDF_3BBB_NO_THIGH_JIGGLE:
      return BodyNameVersion::ASDASDF_3BBB_NO_THIGH_JIGGLE_1_13;
    case BodyVariant::ASDASDF_BBP:
      return BodyNameVersion::ASDASDF_BBP_1_13;
    case BodyVariant::ASDASDF_BBP_NO_THIGH_JIGGLE:
      return BodyNameVersion::ASDASDF_BBP_NO_THIGH_JIGGLE_1_13;
    case BodyVariant::ASDASDF_TBBP:
      return BodyNameVersion::ASDASDF_TBBP_1_13;
    case BodyVariant::ASDASDF_TBBP_NO_THIGH_JIGGLE:
      return BodyNameVersion::ASDASDF_TBBP_NO_THIGH_JIGGLE_1_13;
    case BodyVariant::ASDASDF_ALTERBODY_3BBB:
      return BodyNameVersion::ASDASDF_ALTERBODY_3BBB_1_13;
    case BodyVariant::ASDASDF_ALTERBODY_3BBB_NO_THIGH_JIGGLE:
      return BodyNameVersion::ASDASDF_ALTERBODY_3BBB_NO_THIGH_JIGGLE_1_13;
    case BodyVariant::ASDASDF_ALTERBODY_BBP:
      return BodyNameVersion::ASDASDF_ALTERBODY_BBP_1_13;
    case BodyVariant::ASDASDF_ALTERBODY_BBP_NO_THIGH_JIGGLE:
      return BodyNameVersion::ASDASDF_ALTERBODY_BBP_NO_THIGH_JIGGLE_1_13;
    case BodyVariant::ASDASDF_ALTERBODY_TBBP:
      return BodyNameVersion::ASDASDF_ALTERBODY_TBBP_1_13;
    case BodyVariant::ASDASDF_ALTERBODY_TBBP_NO_THIGH_JIGGLE:
      return BodyNameVersion::ASDASDF_ALTERBODY_TBBP_NO_THIGH_JIGGLE_1_13;
  }

  return BodyNameVersion::_INVALID_VALUE;
}

FeetNameVersion DataLists::GetFirstKey(const FeetVariant& aFeetVariant)
{
  switch (aFeetVariant)
  {
    case FeetVariant::_INVALID_VALUE:
      return FeetNameVersion::_INVALID_VALUE;
    case FeetVariant::CBBE:
      return FeetNameVersion::CBBE_1_6_1;
    case FeetVariant::BHUNP_3BBB_ADVANCED:
      return FeetNameVersion::BHUNP_3BBB_ADVANCED_2_20;
    case FeetVariant::CBBE_3BA_3BBB:
      return FeetNameVersion::CBBE_3BA_3BBB_1_50;
    case FeetVariant::COCO_BODY_NORMAL_CBBE:
      return FeetNameVersion::COCO_BODY_NORMAL_CBBE_V4;
    case FeetVariant::COCO_BODY_NORMAL_UUNP:
      return FeetNameVersion::COCO_BODY_NORMAL_UUNP_V3;
    case FeetVariant::COCO_BODY_HIGH_HEELS_CBBE:
      return FeetNameVersion::COCO_BODY_HIGH_HEELS_CBBE_V6;
    case FeetVariant::COCO_BODY_HIGH_HEELS_UUNP:
      return FeetNameVersion::COCO_BODY_HIGH_HEELS_UUNP_V6;
    case FeetVariant::MIMIR_EBONIC_BODY:
      return FeetNameVersion::MIMIR_EBONIC_BODY_1_2;
    case FeetVariant::ASDASDF:
      return FeetNameVersion::ASDASDF_1_13;
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
  }

  return FeetNameVersion::_INVALID_VALUE;
}

int DataLists::GetVersionOffset(const BodyNameVersion& aBodyNameVersion)
{
  switch (aBodyNameVersion)
  {
    case BodyNameVersion::_INVALID_VALUE:
    case BodyNameVersion::CBBE_3BA_3BBB_1_50:
    case BodyNameVersion::CBBE_3BA_3BBB_1_51_TO_1_55:
    case BodyNameVersion::CBBE_3BA_3BBB_2_02_TO_2_04:
    case BodyNameVersion::CBBE_3BA_3BBB_2_06_AND_2_11_AND_2_12:
    case BodyNameVersion::CBBE_3BA_3BBB_2_13_TO_2_18_AND_2_30_AND_2_31:
    case BodyNameVersion::BHUNP_3BBB_2_20:
    case BodyNameVersion::BHUNP_3BBB_2_25:
    case BodyNameVersion::BHUNP_3BBB_2_30:
    case BodyNameVersion::BHUNP_3BBB_2_31:
    case BodyNameVersion::BHUNP_3BBB_2_35_TO_2_42:
    case BodyNameVersion::BHUNP_3BBB_3_00_TO_3_02:
    case BodyNameVersion::BHUNP_3BBB_3_03:
    case BodyNameVersion::BHUNP_3BBB_3_04:
    case BodyNameVersion::BHUNP_3BBB_ADVANCED_2_20:
    case BodyNameVersion::BHUNP_3BBB_ADVANCED_2_25:
    case BodyNameVersion::BHUNP_3BBB_ADVANCED_2_30:
    case BodyNameVersion::BHUNP_3BBB_ADVANCED_2_31:
    case BodyNameVersion::BHUNP_3BBB_ADVANCED_2_35_TO_2_42:
    case BodyNameVersion::BHUNP_3BBB_ADVANCED_3_00_TO_3_02:
    case BodyNameVersion::BHUNP_3BBB_ADVANCED_3_03:
    case BodyNameVersion::BHUNP_3BBB_ADVANCED_3_04:
    case BodyNameVersion::BHUNP_3BBB_ADVANCED_VER_2_2_20:
    case BodyNameVersion::BHUNP_3BBB_ADVANCED_VER_2_2_25:
    case BodyNameVersion::BHUNP_3BBB_ADVANCED_VER_2_2_30:
    case BodyNameVersion::BHUNP_3BBB_ADVANCED_VER_2_2_31:
    case BodyNameVersion::BHUNP_3BBB_ADVANCED_VER_2_2_35_TO_2_42:
    case BodyNameVersion::BHUNP_3BBB_ADVANCED_VER_2_3_00_TO_3_02:
    case BodyNameVersion::BHUNP_3BBB_ADVANCED_VER_2_3_03:
    case BodyNameVersion::BHUNP_3BBB_ADVANCED_VER_2_3_04:
    case BodyNameVersion::BHUNP_BBP_2_20:
    case BodyNameVersion::BHUNP_BBP_2_25:
    case BodyNameVersion::BHUNP_BBP_2_30:
    case BodyNameVersion::BHUNP_BBP_2_31:
    case BodyNameVersion::BHUNP_BBP_2_35_TO_2_42:
    case BodyNameVersion::BHUNP_BBP_3_00_TO_3_02:
    case BodyNameVersion::BHUNP_BBP_3_03:
    case BodyNameVersion::BHUNP_BBP_3_04:
    case BodyNameVersion::BHUNP_BBP_ADVANCED_2_20:
    case BodyNameVersion::BHUNP_BBP_ADVANCED_2_25:
    case BodyNameVersion::BHUNP_BBP_ADVANCED_2_30:
    case BodyNameVersion::BHUNP_BBP_ADVANCED_2_31:
    case BodyNameVersion::BHUNP_BBP_ADVANCED_2_35_TO_2_42:
    case BodyNameVersion::BHUNP_BBP_ADVANCED_3_00_TO_3_02:
    case BodyNameVersion::BHUNP_BBP_ADVANCED_3_03:
    case BodyNameVersion::BHUNP_BBP_ADVANCED_3_04:
    case BodyNameVersion::BHUNP_TBBP_2_20:
    case BodyNameVersion::BHUNP_TBBP_2_25:
    case BodyNameVersion::BHUNP_TBBP_2_30:
    case BodyNameVersion::BHUNP_TBBP_2_31:
    case BodyNameVersion::BHUNP_TBBP_2_35_TO_2_42:
    case BodyNameVersion::BHUNP_TBBP_3_00_TO_3_02:
    case BodyNameVersion::BHUNP_TBBP_3_03:
    case BodyNameVersion::BHUNP_TBBP_3_04:
    case BodyNameVersion::BHUNP_TBBP_ADVANCED_2_20:
    case BodyNameVersion::BHUNP_TBBP_ADVANCED_2_25:
    case BodyNameVersion::BHUNP_TBBP_ADVANCED_2_30:
    case BodyNameVersion::BHUNP_TBBP_ADVANCED_2_31:
    case BodyNameVersion::BHUNP_TBBP_ADVANCED_2_35_TO_2_42:
    case BodyNameVersion::BHUNP_TBBP_ADVANCED_3_00_TO_3_02:
    case BodyNameVersion::BHUNP_TBBP_ADVANCED_3_03:
    case BodyNameVersion::BHUNP_TBBP_ADVANCED_3_04:
    case BodyNameVersion::CBBE_SMP_3BBB_1_2_0:
    case BodyNameVersion::COCO_BODY_CBBE_V4:
    case BodyNameVersion::COCO_BODY_CBBE_V6:
    case BodyNameVersion::COCO_BODY_CBBE_V6_8:
    case BodyNameVersion::COCO_BODY_UUNP_V3:
    case BodyNameVersion::COCO_BODY_UUNP_V4:
    case BodyNameVersion::COCO_BODY_UUNP_V6:
    case BodyNameVersion::COCO_BODY_UUNP_V6_8:
    case BodyNameVersion::MIMIR_EBONIC_BODY_1_2:
    case BodyNameVersion::MIMIR_EBONIC_BODY_1_2_FOOT_SEAMS_FIX:
    case BodyNameVersion::ASDASDF_3BBB_1_13:
    case BodyNameVersion::ASDASDF_3BBB_NO_THIGH_JIGGLE_1_13:
    case BodyNameVersion::ASDASDF_BBP_1_13:
    case BodyNameVersion::ASDASDF_BBP_NO_THIGH_JIGGLE_1_13:
    case BodyNameVersion::ASDASDF_TBBP_1_13:
    case BodyNameVersion::ASDASDF_TBBP_NO_THIGH_JIGGLE_1_13:
    case BodyNameVersion::ASDASDF_ALTERBODY_3BBB_1_13:
    case BodyNameVersion::ASDASDF_ALTERBODY_3BBB_NO_THIGH_JIGGLE_1_13:
    case BodyNameVersion::ASDASDF_ALTERBODY_BBP_1_13:
    case BodyNameVersion::ASDASDF_ALTERBODY_BBP_NO_THIGH_JIGGLE_1_13:
    case BodyNameVersion::ASDASDF_ALTERBODY_TBBP_1_13:
    case BodyNameVersion::ASDASDF_ALTERBODY_TBBP_NO_THIGH_JIGGLE_1_13:
    {
      return 0;
    }
    case BodyNameVersion::CBBE_3BA_3BBB_NEVERNUDE_2_13_TO_2_18_AND_2_30_AND_2_31:
    case BodyNameVersion::CBBE_3BA_3BBB_UNDERWEAR_2_13_TO_2_18_AND_2_30_AND_2_31:
    {
      return 4; // Ignore "1.50", "1.51 to 1.55", "2.02 to 2.04" and "2.06 - 2.11 - 2.12"
    }
    case BodyNameVersion::BHUNP_3BBB_ADVANCED_VER_3_3_00_TO_3_02:
    case BodyNameVersion::BHUNP_3BBB_ADVANCED_VER_3_3_03:
    case BodyNameVersion::BHUNP_3BBB_ADVANCED_VER_3_3_04:
    {
      return 5; // Ignore "2.20", "2.25", "2.30", "2.31" and "2.35 to 2.42"
    }
    case BodyNameVersion::BHUNP_3BBB_ADVANCED_VER_2_NEVERNUDE_2_25:
    case BodyNameVersion::BHUNP_3BBB_ADVANCED_VER_2_NEVERNUDE_2_30:
    {
      return 1; // Ignore "2.20"
    }
    case BodyNameVersion::BHUNP_3BBB_ADVANCED_VER_2_NEVERNUDE_2_35_TO_2_42:
    case BodyNameVersion::BHUNP_3BBB_ADVANCED_VER_2_NEVERNUDE_3_00_TO_3_02:
    case BodyNameVersion::BHUNP_3BBB_ADVANCED_VER_2_NEVERNUDE_3_03:
    case BodyNameVersion::BHUNP_3BBB_ADVANCED_VER_2_NEVERNUDE_3_04:
    {
      return 2; // Ignore "2.20" and "2.31"
    }
  }

  return 0;
}

int DataLists::GetVersionOffset(const BodyVariant& aBodyVariant, const int aRelativeVersion)
{
  if (aRelativeVersion < 0)
  {
    return 0;
  }

  switch (aBodyVariant)
  {
    case BodyVariant::_INVALID_VALUE:
    case BodyVariant::CBBE_3BA_3BBB:
    case BodyVariant::BHUNP_3BBB:
    case BodyVariant::BHUNP_3BBB_ADVANCED:
    case BodyVariant::BHUNP_3BBB_ADVANCED_VER_2:
    case BodyVariant::BHUNP_BBP:
    case BodyVariant::BHUNP_BBP_ADVANCED:
    case BodyVariant::BHUNP_TBBP:
    case BodyVariant::BHUNP_TBBP_ADVANCED:
    case BodyVariant::CBBE_SMP_3BBB:
    case BodyVariant::COCO_BODY_CBBE:
    case BodyVariant::COCO_BODY_UUNP:
    case BodyVariant::MIMIR_EBONIC_BODY:
    case BodyVariant::ASDASDF_3BBB:
    case BodyVariant::ASDASDF_3BBB_NO_THIGH_JIGGLE:
    case BodyVariant::ASDASDF_BBP:
    case BodyVariant::ASDASDF_BBP_NO_THIGH_JIGGLE:
    case BodyVariant::ASDASDF_TBBP:
    case BodyVariant::ASDASDF_TBBP_NO_THIGH_JIGGLE:
    case BodyVariant::ASDASDF_ALTERBODY_3BBB:
    case BodyVariant::ASDASDF_ALTERBODY_3BBB_NO_THIGH_JIGGLE:
    case BodyVariant::ASDASDF_ALTERBODY_BBP:
    case BodyVariant::ASDASDF_ALTERBODY_BBP_NO_THIGH_JIGGLE:
    case BodyVariant::ASDASDF_ALTERBODY_TBBP:
    case BodyVariant::ASDASDF_ALTERBODY_TBBP_NO_THIGH_JIGGLE:
    {
      return 0;
    }
    case BodyVariant::CBBE_3BA_3BBB_NEVERNUDE:
    case BodyVariant::CBBE_3BA_3BBB_UNDERWEAR:
    {
      return 4; // Ignore "1.50", "1.51 to 1.55", "2.02 to 2.04" and "2.06 - 2.11 - 2.12"
    }
    case BodyVariant::BHUNP_3BBB_ADVANCED_VER_3:
    {
      return 5; // Ignore "2.20", "2.25", "2.30", "2.31" and "2.35 to 2.42"
    }
    case BodyVariant::BHUNP_3BBB_ADVANCED_VER_2_NEVERNUDE:
    {
      // Only for "2.25" and "2.30"
      if (aRelativeVersion == 1 || aRelativeVersion == 2)
      {
        return 1; // Ignore "2.20"
      }

      if (aRelativeVersion == 4                               // Only for 2.35 to 2.42"
          || (aRelativeVersion >= 5 && aRelativeVersion <= 7) // Only for "3.00 to 3.02", "3.03" and "3.04 and 3.05"
      )
      {
        return 2; // Ignore "2.20" and then "2.31"
      }

      return 0; // Default case
    }
  }

  return 0;
}

int DataLists::GetVersionOffset(const FeetVariant& aFeetVariant)
{
  switch (aFeetVariant)
  {
    case FeetVariant::_INVALID_VALUE:
    case FeetVariant::CBBE:
    case FeetVariant::BHUNP_3BBB_ADVANCED:
    case FeetVariant::CBBE_3BA_3BBB:
    case FeetVariant::COCO_BODY_NORMAL_CBBE:
    case FeetVariant::COCO_BODY_NORMAL_UUNP:
    case FeetVariant::MIMIR_EBONIC_BODY:
    case FeetVariant::MORE_SLIDERS_FOR_FEET_NORMAL_CBBE:
    case FeetVariant::MORE_SLIDERS_FOR_FEET_NORMAL_BHUNP:
    case FeetVariant::MORE_SLIDERS_FOR_FEET_HIGH_HEELS_CBBE:
    case FeetVariant::MORE_SLIDERS_FOR_FEET_HIGH_HEELS_BHUNP:
    case FeetVariant::HG_FEET:
    case FeetVariant::KHRYSAMERE_HG_FEET:
    case FeetVariant::ASDASDF:
      return 0;
    case FeetVariant::COCO_BODY_HIGH_HEELS_CBBE:
      return 1; // Ignore "V4"
    case FeetVariant::COCO_BODY_HIGH_HEELS_UUNP:
      return 2; // Ignore "V3" and "V4"
  }

  return 0;
}

QString DataLists::GetQRCPathForResource(const BodyNameVersion& aBodyNameVersion, const FeetNameVersion& aFeetNameVersion, const BodyPartType& aResourceType)
{
  QString lResourceType;
  QString lResourceName;
  QString lResourceVersion;

  switch (aResourceType)
  {
    case BodyPartType::BODY:
    {
      lResourceType = "body";
      lResourceName = DataLists::GetQRCResourceName(DataLists::GetVariant(aBodyNameVersion));
      lResourceVersion = DataLists::GetVersionString(aBodyNameVersion);
      break;
    }
    case BodyPartType::FEET:
    {
      lResourceType = "feet";
      lResourceName = DataLists::GetQRCResourceName(DataLists::GetVariant(aBodyNameVersion), aFeetNameVersion);
      lResourceVersion = DataLists::GetVersionString(aBodyNameVersion, aFeetNameVersion);
      break;
    }
    case BodyPartType::HANDS:
    {
      lResourceType = "hands";
      lResourceVersion = DataLists::GetVersionString(aBodyNameVersion);

      const auto lBodyName{DataLists::GetName(aBodyNameVersion)};
      switch (lBodyName)
      {
        case BodyName::_INVALID_VALUE:
        {
          lResourceVersion = QString();
          break;
        }
        case BodyName::CBBE_3BA_3BBB:
        {
          lResourceName = QString("cbbe 3bbb 3ba");
          break;
        }
        case BodyName::BHUNP_UUNP_NEXT_GENERATION:
        {
          const auto lBodyVariant{DataLists::GetVariant(aBodyNameVersion)};
          if (lBodyVariant == BodyVariant::BHUNP_3BBB_ADVANCED_VER_3)
          {
            lResourceName = QString("bhunp ver 3");
          }
          else
          {
            lResourceName = QString("bhunp");
          }
          break;
        }
        case BodyName::CBBE_SMP_3BBB:
        {
          lResourceName = QString("cbbe");
          lResourceVersion = QString("1.6.1"); // Default CBBE hands
          break;
        }
        case BodyName::COCO_BODY_CBBE:
        {
          lResourceName = QString("coco body cbbe");
          break;
        }
        case BodyName::COCO_BODY_UUNP:
        {
          lResourceName = QString("coco body uunp");
          break;
        }
        case BodyName::MIMIR_EBONIC_BODY:
        {
          lResourceName = QString("mimir ebonic body");
          break;
        }
        case BodyName::BHUNP_LITE_ASDASDF:
        {
          lResourceName = QString("asdasdf");
          break;
        }
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

  return QString(":/presets/%1/%2/%3").arg(lResourceType, lResourceName, lResourceVersion);
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
  }

  return std::pair<int, int>(-1, -1);
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
    }
  }
  else
  {
    switch (aFeetIndex)
    {
      // Default
      case 0:
        lFeetToSet = DataLists::GetFirstKey(FeetVariant::BHUNP_3BBB_ADVANCED);
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
      // Khrysamere HG Feet (Claws) (BHUNP)
      case 5:
        lFeetToSet = DataLists::GetFirstKey(FeetVariant::KHRYSAMERE_HG_FEET);
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

QStringList DataLists::GetKnownTexturesFilesNames()
{
  return QStringList({// Body
                      QString("femalebody_1"),
                      QString("femalebody_1_s"),
                      QString("femalebody_1_sk"),
                      QString("femalebody_1_msn"),
                      // Body extra
                      QString("femalebody_etc_1"),
                      QString("femalebody_etc_1_s"),
                      QString("femalebody_etc_1_sk"),
                      QString("femalebody_etc_1_msn"),
                      QString("femalebody_etc_v2_1"),
                      QString("femalebody_etc_v2_1_s"),
                      QString("femalebody_etc_v2_1_sk"),
                      QString("femalebody_etc_v2_1_msn"),
                      // Hands
                      QString("femalehands_1"),
                      QString("femalehands_1_s"),
                      QString("femalehands_1_sk"),
                      QString("femalehands_1_msn"),
                      // Head
                      QString("femalehead"),
                      QString("femalehead_s"),
                      QString("femalehead_sk"),
                      QString("femalehead_msn"),
                      QString("femaleheadvampire"),
                      QString("femaleheadvampire_msn"),
                      QString("femaleheaddetail_frekles"),
                      QString("femaleheaddetail_rough"),
                      // Mouth
                      QString("mouthhuman"),
                      QString("mouthhuman_n")});
}

QString DataLists::GetBodySliderValue(const BodyNameVersion& aBodyNameVersion)
{
  const auto lBodyVariant{DataLists::GetVariant(aBodyNameVersion)};
  const auto lVersionIndex{DataLists::GetVersionIndex(aBodyNameVersion)};

  switch (lBodyVariant)
  {
    case BodyVariant::CBBE_3BA_3BBB:
    case BodyVariant::MIMIR_EBONIC_BODY:
      return QString("CBBE 3BBB Body Amazing");
    case BodyVariant::CBBE_3BA_3BBB_NEVERNUDE:
      return QString("CBBE 3BBB Amazing NeverNude");
    case BodyVariant::CBBE_3BA_3BBB_UNDERWEAR:
      return QString("CBBE 3BBB Amazing Underwear");
    case BodyVariant::BHUNP_3BBB:
      return QString("BHUNP 3BBB");
    case BodyVariant::BHUNP_3BBB_ADVANCED:
      return QString("BHUNP 3BBB Advanced");
    case BodyVariant::BHUNP_3BBB_ADVANCED_VER_2:
      return QString("BHUNP 3BBB Advanced Ver 2");
    case BodyVariant::BHUNP_3BBB_ADVANCED_VER_3:
      return QString("BHUNP 3BBB Advanced Ver 3");
    case BodyVariant::BHUNP_BBP:
      return QString("BHUNP BBP");
    case BodyVariant::BHUNP_BBP_ADVANCED:
      return QString("BHUNP BBP Advanced");
    case BodyVariant::BHUNP_TBBP:
      return QString("BHUNP TBBP");
    case BodyVariant::BHUNP_TBBP_ADVANCED:
      return QString("BHUNP TBBP Advanced");
    case BodyVariant::BHUNP_3BBB_ADVANCED_VER_2_NEVERNUDE:
      return QString("BHUNP 3BBB Advanced Ver 2 Nevernude");
    case BodyVariant::CBBE_SMP_3BBB:
      return QString("CBBE Body SMP (3BBB)");
    case BodyVariant::COCO_BODY_CBBE:
      switch (lVersionIndex)
      {
        case 0:
          return QString("[COCO]3Bsmp_BodyV4_A");
        case 1:
          return QString("[COCO 3BBB V6]Body_A");
      }
      return QString();
    case BodyVariant::COCO_BODY_UUNP:
      switch (lVersionIndex)
      {
        case 0:
          return QString("[COCO]body_B_v3");
        case 1:
          return QString("[COCO]3Bsmp_BodyV4_B");
        case 2:
          return QString("[COCO 3BBB V6]Body_B");
      }
      return QString();
    case BodyVariant::ASDASDF_3BBB:
      return QString("ASDASDF-BHUNP 3BBB");
    case BodyVariant::ASDASDF_3BBB_NO_THIGH_JIGGLE:
      return QString("ASDASDF-BHUNP 3BBB - No Thigh Jiggle");
    case BodyVariant::ASDASDF_BBP:
      return QString("ASDASDF-BHUNP BBP");
    case BodyVariant::ASDASDF_BBP_NO_THIGH_JIGGLE:
      return QString("ASDASDF-BHUNP BBP - No Thigh Jiggle");
    case BodyVariant::ASDASDF_TBBP:
      return QString("ASDASDF-BHUNP TBBP");
    case BodyVariant::ASDASDF_TBBP_NO_THIGH_JIGGLE:
      return QString("ASDASDF-BHUNP TBBP - No Thigh Jiggle");
    case BodyVariant::ASDASDF_ALTERBODY_3BBB:
      return QString("ASDASDF-BHUNP_AlterBody 3BBB");
    case BodyVariant::ASDASDF_ALTERBODY_3BBB_NO_THIGH_JIGGLE:
      return QString("ASDASDF-BHUNP_AlterBody 3BBB - No Thigh Jiggle");
    case BodyVariant::ASDASDF_ALTERBODY_BBP:
      return QString("ASDASDF-BHUNP_AlterBody BBP");
    case BodyVariant::ASDASDF_ALTERBODY_BBP_NO_THIGH_JIGGLE:
      return QString("ASDASDF-BHUNP_AlterBody BBP - No Thigh Jiggle");
    case BodyVariant::ASDASDF_ALTERBODY_TBBP:
      return QString("ASDASDF-BHUNP_AlterBody TBBP");
    case BodyVariant::ASDASDF_ALTERBODY_TBBP_NO_THIGH_JIGGLE:
      return QString("ASDASDF-BHUNP_AlterBody TBBP - No Thigh Jiggle");
    case BodyVariant::_INVALID_VALUE:
      return QString();
  }

  return QString();
}

QString DataLists::GetFeetSliderValue(const FeetNameVersion& aFeetNameVersion)
{
  const auto lVariant{DataLists::GetVariant(aFeetNameVersion)};
  const auto lVersionIndex{DataLists::GetVersionIndex(aFeetNameVersion)};

  switch (lVariant)
  {
    case FeetVariant::CBBE:
      return QString("CBBE Feet");
    case FeetVariant::BHUNP_3BBB_ADVANCED:
      return QString("BHUNP 3BBB Advanced Feet");
    case FeetVariant::CBBE_3BA_3BBB:
      return QString("CBBE 3BBB Feet");
    case FeetVariant::COCO_BODY_NORMAL_CBBE:
      switch (lVersionIndex)
      {
        case 0: // V4
          return QString("[COCO]3Bsmp_FeetV4_A");
        case 1: // V6
        case 2: // V6.8
          return QString("[COCO 3BBB V6]Feet_A");
      }
      return QString();
    case FeetVariant::COCO_BODY_NORMAL_UUNP:
      switch (lVersionIndex)
      {
        case 0: // V3
          return QString("[COCO]bodyFeets_B_v3");
        case 1: // V4
          return QString("[COCO]3Bsmp_FeetV4_B");
        case 2: // V6
        case 3: // V6.8
          return QString("[COCO 3BBB V6]Feet_B");
      }
      return QString();
    case FeetVariant::COCO_BODY_HIGH_HEELS_CBBE:
      // V6 or V6.8
      return QString("[COCO]CBBEV6_HH100");
    case FeetVariant::COCO_BODY_HIGH_HEELS_UUNP:
      // V6 or V6.8
      return QString("[COCO]UUNPV6_HH100");
    case FeetVariant::MIMIR_EBONIC_BODY:
      return QString("CBBE 3BBB Feet");
    case FeetVariant::MORE_SLIDERS_FOR_FEET_NORMAL_CBBE:
      return QString("CBBE MSF Feet");
    case FeetVariant::MORE_SLIDERS_FOR_FEET_NORMAL_BHUNP:
      return QString("BHUNP Feet MSF");
    case FeetVariant::MORE_SLIDERS_FOR_FEET_HIGH_HEELS_CBBE:
      return QString("CBBE MSF Feet High Heel");
    case FeetVariant::MORE_SLIDERS_FOR_FEET_HIGH_HEELS_BHUNP:
      return QString("BHUNP Feet High Heel MSF");
    case FeetVariant::HG_FEET:
      return QString("HGFeet UUNP");
    case FeetVariant::KHRYSAMERE_HG_FEET:
      switch (lVersionIndex)
      {
        case 0:
          return QString("[Khrysamere] HG Feet (BHUNP)");
        case 1:
          return QString("[Khrysamere] K Feet");
        case 2:
          return QString("[Khrysamere] K Feet");
        case 3:
          return QString("[Khrysamere] K Feet v5.0");
        case 4:
          return QString("[Khrysamere] K Feet");
      }
      return QString();
    case FeetVariant::ASDASDF:
      return QString("ASDASDF-BHUNP Feet");
    case FeetVariant::_INVALID_VALUE:
      return QString();
  }

  return QString();
}

QString DataLists::GetHandsSliderValue(const BodyNameVersion& aBodyNameVersion, const bool aMustUseBeastHands)
{
  // Beast hands
  if (Utils::IsCBBEBasedBody(aBodyNameVersion) && aMustUseBeastHands)
  {
    return QString("CBBE Hands Beast");
  }

  const auto lBodyName{DataLists::GetName(aBodyNameVersion)};
  const auto lVersionIndex{DataLists::GetVersionIndex(aBodyNameVersion)};

  switch (lBodyName)
  {
    case BodyName::CBBE_3BA_3BBB:
    case BodyName::MIMIR_EBONIC_BODY:
    {
      return QString("CBBE 3BBB Hands");
    }
    case BodyName::BHUNP_UUNP_NEXT_GENERATION:
    {
      // TODO: Remove this workaround once the user can choose hands meshes
      const auto lBodyVariant{DataLists::GetVariant(aBodyNameVersion)};
      if (lBodyVariant == BodyVariant::BHUNP_3BBB_ADVANCED_VER_3
          && (lVersionIndex >= 5 && lVersionIndex <= 7) // Only for "3.00 to 3.02", "3.03" and "3.04 and 3.05"
      )
      {
        return QString("BHUNP 3BBB Advanced Hands Ver 3");
      }

      return QString("BHUNP 3BBB Advanced Hands");
    }
    case BodyName::CBBE_SMP_3BBB:
    {
      return QString("CBBE Hands");
    }
    case BodyName::COCO_BODY_CBBE:
    {
      switch (lVersionIndex)
      {
        case 0:
          return QString("[COCO]3Bsmp_HandV4_A");
        case 1:
          return QString("[COCO 3BBB V6]Hands_A");
      }

      break;
    }
    case BodyName::COCO_BODY_UUNP:
    {
      switch (lVersionIndex)
      {
        case 0:
          return QString("[COCO]bodyHands_B_v3");
        case 1:
          return QString("[COCO]3Bsmp_HandV4_B");
        case 2:
          return QString("[COCO 3BBB V6]Hands_B");
      }

      break;
    }
    case BodyName::BHUNP_LITE_ASDASDF:
    {
      return QString("ASDASDF-BHUNP Hands");
    }
    case BodyName::_INVALID_VALUE:
    {
      return QString();
    }
  }

  return QString();
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
  return QStringList{QString("Default OS theme"),
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
  return QStringList{QString("general"),
                     QString("mainWindowOutput"),
                     QString("batchConversionInput"),
                     QString("batchConversionOutput"),
                     QString("assistedConversionInput"),
                     QString("retargetingToolInput"),
                     QString("retargetingToolOutput"),
                     QString("texturesAssistantInput"),
                     QString("texturesAssistantOutput"),
                     QString("lastLoadedProject"),
                     QString("lastSavedProject")};
}
