#include "DataLists.h"
#include "Utils.h"

QStringList DataLists::GetBodyNamesList()
{
  return QStringList{QStringLiteral("CBBE 3BA (3BBB) - by Acro"),
                     QStringLiteral("BHUNP (UUNP Next Generation) SSE - by Bakafactory and Haeun"),
                     QStringLiteral("CBBE SMP (3BBB) - by Ousnius"),
                     QStringLiteral("COCO Body CBBE - by COCO"),
                     QStringLiteral("COCO Body UUNP - by COCO"),
                     QStringLiteral("Mimir Ebonic Body V.2 - by Medley"),
                     QStringLiteral("BHUNP Lite - AKA ASDASDF Body - by Myst")};
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
      return QStringList({QStringLiteral("1.50"),
                          QStringLiteral("1.51 to 1.55"),
                          QStringLiteral("2.02 to 2.04"),
                          QStringLiteral("2.06 - 2.11 - 2.12"),
                          QStringLiteral("2.13 to 2.22 - 2.30 - 2.31"),
                          QStringLiteral("2.40 to 2.42")});
    }
    case BodyName::BHUNP_UUNP_NEXT_GENERATION:
    {
      return QStringList({QStringLiteral("2.20"),
                          QStringLiteral("2.25"),
                          QStringLiteral("2.30"),
                          QStringLiteral("2.31"),
                          QStringLiteral("2.35 to 2.42"),
                          QStringLiteral("3.00 to 3.02"),
                          QStringLiteral("3.03"),
                          QStringLiteral("3.04 - 3.05"),
                          QStringLiteral("3.10"),
                          QStringLiteral("3.11 to 3.13")});
    }
    case BodyName::CBBE_SMP_3BBB:
    {
      return QStringList(QStringLiteral("1.2.0"));
    }
    case BodyName::COCO_BODY_CBBE:
    {
      return QStringList({QStringLiteral("V4"), QStringLiteral("V6"), QStringLiteral("V6.8"), QStringLiteral("V6.9 - V6.95")});
    }
    case BodyName::COCO_BODY_UUNP:
    {
      return QStringList({QStringLiteral("V3"), QStringLiteral("V4"), QStringLiteral("V6"), QStringLiteral("V6.8"), QStringLiteral("V6.9 - V6.95")});
    }
    case BodyName::MIMIR_EBONIC_BODY:
    {
      return QStringList({QStringLiteral("1.2"), QStringLiteral("1.2 foot fix")});
    }
    case BodyName::BHUNP_LITE_ASDASDF:
    {
      return QStringList(QStringLiteral("1.13"));
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
      QStringList lVariantsList{QStringLiteral("CBBE 3BBB Body Amazing")};

      // Only for "2.13 to 2.22 - 2.30 - 2.31" and "CBBE 3BA 3BBB 2.40 to 2.42"
      if (aRelativeVersion >= 4 && aRelativeVersion <= 5)
      {
        lVariantsList.append(QStringLiteral("CBBE 3BBB Amazing NeverNude"));
        lVariantsList.append(QStringLiteral("CBBE 3BBB Amazing Underwear"));
      }

      return lVariantsList;
    }
    case BodyName::BHUNP_UUNP_NEXT_GENERATION:
    {
      QStringList lVariantsList;

      // All the version numbers propose the variant below:
      lVariantsList.append(QStringLiteral("BHUNP 3BBB"));
      lVariantsList.append(QStringLiteral("BHUNP 3BBB Advanced"));
      lVariantsList.append(QStringLiteral("BHUNP 3BBB Advanced Ver 2"));

      // Only for "3.00 to 3.02", "3.03", "3.04 - 3.05", "3.10" and "3.11 to 3.13"
      if (aRelativeVersion >= 5 && aRelativeVersion <= 9)
        lVariantsList.append(QStringLiteral("BHUNP 3BBB Advanced Ver 3"));

      // All the version numbers propose the variant below:
      lVariantsList.append(QStringLiteral("BHUNP BBP"));
      lVariantsList.append(QStringLiteral("BHUNP BBP Advanced"));
      lVariantsList.append(QStringLiteral("BHUNP TBBP"));
      lVariantsList.append(QStringLiteral("BHUNP TBBP Advanced"));

      // If "2.20" or "2.31"
      if (aRelativeVersion == 0 || aRelativeVersion == 3)
      {
        lVariantsList.append(QStringLiteral("BHUNP 3BBB Advanced Ver 2 (Nevernude)"));
      }

      return lVariantsList;
    }
    case BodyName::CBBE_SMP_3BBB:
    {
      // All the version numbers propose the variant below:
      return QStringList{QStringLiteral("CBBE Body SMP (3BBB)")};
    }
    case BodyName::COCO_BODY_CBBE:
    {
      // All the version numbers propose the variant below:
      return QStringList{QStringLiteral("COCO Body CBBE")};
    }
    case BodyName::COCO_BODY_UUNP:
    {
      // All the version numbers propose the variant below:
      return QStringList{QStringLiteral("COCO Body UUNP")};
    }
    case BodyName::MIMIR_EBONIC_BODY:
    {
      // All the version numbers propose the variant below:
      return QStringList{QStringLiteral("Mimir Ebonic Body")};
    }
    case BodyName::BHUNP_LITE_ASDASDF:
    {
      // All the version numbers propose the variants below:
      return QStringList{QStringLiteral("ASDASDF 3BBB"),
                         QStringLiteral("ASDASDF 3BBB - NO THIGH JIGGLE"),
                         QStringLiteral("ASDASDF BBP"),
                         QStringLiteral("ASDASDF BBP - NO THIGH JIGGLE"),
                         QStringLiteral("ASDASDF TBBP"),
                         QStringLiteral("ASDASDF TBBP - NO THIGH JIGGLE"),
                         QStringLiteral("ASDASDF ALTERBODY 3BBB"),
                         QStringLiteral("ASDASDF ALTERBODY 3BBB - NO THIGH JIGGLE"),
                         QStringLiteral("ASDASDF ALTERBODY BBP"),
                         QStringLiteral("ASDASDF ALTERBODY BBP - NO THIGH JIGGLE"),
                         QStringLiteral("ASDASDF ALTERBODY TBBP"),
                         QStringLiteral("ASDASDF ALTERBODY TBBP - NO THIGH JIGGLE")};
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
      return QStringLiteral("cbbe 3bbb 3ba");
    case BodyVariant::CBBE_3BA_3BBB_NEVERNUDE:
      return QStringLiteral("cbbe 3bbb 3ba nevernude");
    case BodyVariant::CBBE_3BA_3BBB_UNDERWEAR:
      return QStringLiteral("cbbe 3bbb 3ba underwear");
    case BodyVariant::BHUNP_3BBB:
      return QStringLiteral("bhunp 3bbb");
    case BodyVariant::BHUNP_3BBB_ADVANCED:
      return QStringLiteral("bhunp 3bbb advanced");
    case BodyVariant::BHUNP_3BBB_ADVANCED_VER_2:
      return QStringLiteral("bhunp 3bbb advanced ver 2");
    case BodyVariant::BHUNP_3BBB_ADVANCED_VER_3:
      return QStringLiteral("bhunp 3bbb advanced ver 3");
    case BodyVariant::BHUNP_BBP:
      return QStringLiteral("bhunp bbp");
    case BodyVariant::BHUNP_BBP_ADVANCED:
      return QStringLiteral("bhunp bbp advanced");
    case BodyVariant::BHUNP_TBBP:
      return QStringLiteral("bhunp tbbp");
    case BodyVariant::BHUNP_TBBP_ADVANCED:
      return QStringLiteral("bhunp tbbp advanced");
    case BodyVariant::BHUNP_3BBB_ADVANCED_VER_2_NEVERNUDE:
      return QStringLiteral("bhunp 3bbb advanced ver 2 nevernude");
    case BodyVariant::CBBE_SMP_3BBB:
      return QStringLiteral("cbbe smp 3bbb");
    case BodyVariant::COCO_BODY_CBBE:
      return QStringLiteral("coco body cbbe");
    case BodyVariant::COCO_BODY_UUNP:
      return QStringLiteral("coco body uunp");
    case BodyVariant::MIMIR_EBONIC_BODY:
      return QStringLiteral("mimir ebonic body");
    case BodyVariant::ASDASDF_3BBB:
      return QStringLiteral("asdasdf 3bbb");
    case BodyVariant::ASDASDF_3BBB_NO_THIGH_JIGGLE:
      return QStringLiteral("asdasdf 3bbb - ntj");
    case BodyVariant::ASDASDF_BBP:
      return QStringLiteral("asdasdf bbp");
    case BodyVariant::ASDASDF_BBP_NO_THIGH_JIGGLE:
      return QStringLiteral("asdasdf bbp - ntj");
    case BodyVariant::ASDASDF_TBBP:
      return QStringLiteral("asdasdf tbbp");
    case BodyVariant::ASDASDF_TBBP_NO_THIGH_JIGGLE:
      return QStringLiteral("asdasdf tbbp - ntj");
    case BodyVariant::ASDASDF_ALTERBODY_3BBB:
      return QStringLiteral("asdasdf alterbody 3bbb");
    case BodyVariant::ASDASDF_ALTERBODY_3BBB_NO_THIGH_JIGGLE:
      return QStringLiteral("asdasdf alterbody 3bbb - ntj");
    case BodyVariant::ASDASDF_ALTERBODY_BBP:
      return QStringLiteral("asdasdf alterbody bbp");
    case BodyVariant::ASDASDF_ALTERBODY_BBP_NO_THIGH_JIGGLE:
      return QStringLiteral("asdasdf alterbody bbp - ntj");
    case BodyVariant::ASDASDF_ALTERBODY_TBBP:
      return QStringLiteral("asdasdf alterbody tbbp");
    case BodyVariant::ASDASDF_ALTERBODY_TBBP_NO_THIGH_JIGGLE:
      return QStringLiteral("asdasdf alterbody tbbp - ntj");
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
      return QStringList{QStringLiteral("CBBE 3BBB Feet - by Acro"),
                         QStringLiteral("More Sliders for Feet - by Balveric")};
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
      return QStringList{QStringLiteral("BHUNP 3BBB Advanced Feet - by Bakafactory and Haeun"),
                         QStringLiteral("More Sliders for Feet - by Balveric"),
                         QStringLiteral("HG Feet and Toes UNP - by SunJeong"),
                         QStringLiteral("K Feet - by Khrysamere")};
    }
    case BodyVariant::CBBE_SMP_3BBB:
    {
      return QStringList{QStringLiteral("CBBE Default Feet - by Ousnius"),
                         QStringLiteral("More Sliders for Feet - by Balveric")};
    }
    case BodyVariant::COCO_BODY_CBBE:
    {
      return QStringList{QStringLiteral("COCO Feet - by COCO"),
                         QStringLiteral("More Sliders for Feet - by Balveric")};
    }
    case BodyVariant::COCO_BODY_UUNP:
    {
      return QStringList{QStringLiteral("COCO Feet - by COCO"),
                         QStringLiteral("More Sliders for Feet - by Balveric"),
                         QStringLiteral("HG Feet and Toes UNP - by SunJeong"),
                         QStringLiteral("K Feet - by Khrysamere")};
    }
    case BodyVariant::MIMIR_EBONIC_BODY:
    {
      return QStringList{QStringLiteral("CBBE 3BBB Feet - by Medley"),
                         QStringLiteral("More Sliders for Feet - by Balveric")};
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
      return QStringList{QStringLiteral("ASDASDF BHUNP Feet - by Myst"),
                         QStringLiteral("More Sliders for Feet - by Balveric"),
                         QStringLiteral("HG Feet and Toes UNP - by SunJeong"),
                         QStringLiteral("K Feet - by Khrysamere")};
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
      return QStringList{QStringLiteral("1.6.1")};
    case FeetName::BHUNP_3BBB_ADVANCED:
      return QStringList{QStringLiteral("2.20"),
                         QStringLiteral("2.25"),
                         QStringLiteral("2.30"),
                         QStringLiteral("2.31"),
                         QStringLiteral("2.35 to 2.42"),
                         QStringLiteral("3.00 to 3.02"),
                         QStringLiteral("3.03"),
                         QStringLiteral("3.04 - 3.05"),
                         QStringLiteral("3.10"),
                         QStringLiteral("3.11 to 3.13")};
    case FeetName::CBBE_3BA_3BBB:
      return QStringList{QStringLiteral("1.50"),
                         QStringLiteral("1.51 to 1.55"),
                         QStringLiteral("2.02 to 2.04"),
                         QStringLiteral("2.06 - 2.11 - 2.12"),
                         QStringLiteral("2.13 to 2.22 - 2.30 - 2.31"),
                         QStringLiteral("2.40 to 2.42")};
    case FeetName::COCO_BODY:
      if (aIsCBBEBody)
        return QStringList{QStringLiteral("V4"), QStringLiteral("V6"), QStringLiteral("V6.8"), QStringLiteral("V6.9 - V6.95")};

      return QStringList{QStringLiteral("V3"), QStringLiteral("V4"), QStringLiteral("V6"), QStringLiteral("V6.8"), QStringLiteral("V6.9 - V6.95")};
    case FeetName::MIMIR_EBONIC_BODY:
      return QStringList{QStringLiteral("1.2"), QStringLiteral("1.2 foot fix")};
    case FeetName::MORE_SLIDERS_FOR_FEET:
      if (aIsCBBEBody)
        return QStringList{QStringLiteral("1.0")};
      return QStringList{QStringLiteral("1.1")};
    case FeetName::HG_FEET:
      return QStringList{QStringLiteral("1")};
    case FeetName::KHRYSAMERE_HG_FEET:
      return QStringList{QStringLiteral("1.0"), QStringLiteral("1.3.3"), QStringLiteral("1.4"), QStringLiteral("1.5"), QStringLiteral("1.6")};
    case FeetName::ASDASDF:
      return QStringList{QStringLiteral("1.13")};
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
      return QStringList{QStringLiteral("CBBE Default Feet")};
    }
    case FeetName::BHUNP_3BBB_ADVANCED:
    {
      return QStringList{QStringLiteral("BHUNP 3BBB Advanced Feet")};
    }
    case FeetName::CBBE_3BA_3BBB:
    {
      return QStringList{QStringLiteral("CBBE 3BBB Feet")};
    }
    case FeetName::COCO_BODY:
    {
      // All the version numbers propose the variants below:
      QStringList lVariantsList{QStringLiteral("COCO Feet - Normal")};

      // Only for "V6", "V6.8" and "V6.9 - V6.95"
      if ((aIsCBBEBody && aRelativeVersion >= 1)      // CBBE
          || (!aIsCBBEBody && aRelativeVersion >= 2)) // UUNP
      {
        lVariantsList.append(QStringLiteral("COCO Feet - High Heels"));
      }

      return lVariantsList;
    }
    case FeetName::MIMIR_EBONIC_BODY:
    {
      return QStringList{QStringLiteral("CBBE 3BBB Feet")};
    }
    case FeetName::MORE_SLIDERS_FOR_FEET:
    {
      return QStringList{QStringLiteral("More Sliders for Feet - Normal"),
                         QStringLiteral("More Sliders for Feet - High Heels")};
    }
    case FeetName::HG_FEET:
    {
      return QStringList{QStringLiteral("HGFeet UUNP")};
    }
    case FeetName::KHRYSAMERE_HG_FEET:
    {
      return QStringList{QStringLiteral("K Feet")};
    }
    case FeetName::ASDASDF:
    {
      return QStringList{QStringLiteral("ASDASDF BHUNP Feet")};
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
      return QStringLiteral("cbbe");
    }
    case FeetName::BHUNP_3BBB_ADVANCED:
    {
      return QStringLiteral("bhunp");
    }
    case FeetName::CBBE_3BA_3BBB:
    {
      return QStringLiteral("cbbe 3bbb 3ba");
    }
    case FeetName::COCO_BODY:
    {
      const auto lFeetVariant{DataLists::GetVariant(aFeetNameVersion)};

      // CBBE
      if (Utils::IsCBBEBasedBody(aBodyVariant))
      {
        if (lFeetVariant == FeetVariant::COCO_BODY_HIGH_HEELS_CBBE)
        {
          return QStringLiteral("coco body/high heels/cbbe");
        }

        return QStringLiteral("coco body/normal/cbbe");
      }

      // UUNP
      if (lFeetVariant == FeetVariant::COCO_BODY_HIGH_HEELS_UUNP)
      {
        return QStringLiteral("coco body/high heels/uunp");
      }

      return QStringLiteral("coco body/normal/uunp");
    }
    case FeetName::MIMIR_EBONIC_BODY:
    {
      return QStringLiteral("mimir ebonic body");
    }
    case FeetName::MORE_SLIDERS_FOR_FEET:
    {
      if (aFeetNameVersion == FeetNameVersion::MORE_SLIDERS_FOR_FEET_NORMAL_CBBE_1_0)
        return QStringLiteral("msf/normal/cbbe");
      else if (aFeetNameVersion == FeetNameVersion::MORE_SLIDERS_FOR_FEET_NORMAL_BHUNP_1_1)
        return QStringLiteral("msf/normal/bhunp");
      else if (aFeetNameVersion == FeetNameVersion::MORE_SLIDERS_FOR_FEET_HIGH_HEELS_CBBE_1_0)
        return QStringLiteral("msf/high heels/cbbe");
      else if (aFeetNameVersion == FeetNameVersion::MORE_SLIDERS_FOR_FEET_HIGH_HEELS_BHUNP_1_1)
        return QStringLiteral("msf/high heels/bhunp");

      break;
    }
    case FeetName::HG_FEET:
    {
      if (!Utils::IsCBBEBasedBody(aBodyVariant))
      {
        return QStringLiteral("hg feet/bhunp");
      }

      break;
    }
    case FeetName::KHRYSAMERE_HG_FEET:
    {
      return QStringLiteral("khrysamere hg feet");
    }
    case FeetName::ASDASDF:
    {
      return QStringLiteral("asdasdf");
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
    case BodyNameVersion::CBBE_3BA_3BBB_2_40_TO_2_42:
      return BodyVariant::CBBE_3BA_3BBB;
    //
    case BodyNameVersion::CBBE_3BA_3BBB_NEVERNUDE_2_13_TO_2_18_AND_2_30_AND_2_31:
    case BodyNameVersion::CBBE_3BA_3BBB_NEVERNUDE_2_40_TO_2_42:
      return BodyVariant::CBBE_3BA_3BBB_NEVERNUDE;
    //
    case BodyNameVersion::CBBE_3BA_3BBB_UNDERWEAR_2_13_TO_2_18_AND_2_30_AND_2_31:
    case BodyNameVersion::CBBE_3BA_3BBB_UNDERWEAR_2_40_TO_2_42:
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
    case BodyNameVersion::BHUNP_3BBB_3_10:
    case BodyNameVersion::BHUNP_3BBB_3_11_TO_3_13:
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
    case BodyNameVersion::BHUNP_3BBB_ADVANCED_3_10:
    case BodyNameVersion::BHUNP_3BBB_ADVANCED_3_11_TO_3_13:
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
    case BodyNameVersion::BHUNP_3BBB_ADVANCED_VER_2_3_10:
    case BodyNameVersion::BHUNP_3BBB_ADVANCED_VER_2_3_11_TO_3_13:
      return BodyVariant::BHUNP_3BBB_ADVANCED_VER_2;
      //
    case BodyNameVersion::BHUNP_3BBB_ADVANCED_VER_3_3_00_TO_3_02:
    case BodyNameVersion::BHUNP_3BBB_ADVANCED_VER_3_3_03:
    case BodyNameVersion::BHUNP_3BBB_ADVANCED_VER_3_3_04:
    case BodyNameVersion::BHUNP_3BBB_ADVANCED_VER_3_3_10:
    case BodyNameVersion::BHUNP_3BBB_ADVANCED_VER_3_3_11_TO_3_13:
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
    case BodyNameVersion::BHUNP_BBP_3_10:
    case BodyNameVersion::BHUNP_BBP_3_11_TO_3_13:
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
    case BodyNameVersion::BHUNP_BBP_ADVANCED_3_10:
    case BodyNameVersion::BHUNP_BBP_ADVANCED_3_11_TO_3_13:
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
    case BodyNameVersion::BHUNP_TBBP_3_10:
    case BodyNameVersion::BHUNP_TBBP_3_11_TO_3_13:
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
    case BodyNameVersion::BHUNP_TBBP_ADVANCED_3_10:
    case BodyNameVersion::BHUNP_TBBP_ADVANCED_3_11_TO_3_13:
      return BodyVariant::BHUNP_TBBP_ADVANCED;
    //
    case BodyNameVersion::BHUNP_3BBB_ADVANCED_VER_2_NEVERNUDE_2_25:
    case BodyNameVersion::BHUNP_3BBB_ADVANCED_VER_2_NEVERNUDE_2_30:
    case BodyNameVersion::BHUNP_3BBB_ADVANCED_VER_2_NEVERNUDE_2_35_TO_2_42:
    case BodyNameVersion::BHUNP_3BBB_ADVANCED_VER_2_NEVERNUDE_3_00_TO_3_02:
    case BodyNameVersion::BHUNP_3BBB_ADVANCED_VER_2_NEVERNUDE_3_03:
    case BodyNameVersion::BHUNP_3BBB_ADVANCED_VER_2_NEVERNUDE_3_04:
    case BodyNameVersion::BHUNP_3BBB_ADVANCED_VER_2_NEVERNUDE_3_10:
    case BodyNameVersion::BHUNP_3BBB_ADVANCED_VER_2_NEVERNUDE_3_11_TO_3_13:
      return BodyVariant::BHUNP_3BBB_ADVANCED_VER_2_NEVERNUDE;
    //
    case BodyNameVersion::CBBE_SMP_3BBB_1_2_0:
      return BodyVariant::CBBE_SMP_3BBB;
    //
    case BodyNameVersion::COCO_BODY_CBBE_V4:
    case BodyNameVersion::COCO_BODY_CBBE_V6:
    case BodyNameVersion::COCO_BODY_CBBE_V6_8:
    case BodyNameVersion::COCO_BODY_CBBE_V6_9_AND_V6_95:
      return BodyVariant::COCO_BODY_CBBE;
    //
    case BodyNameVersion::COCO_BODY_UUNP_V3:
    case BodyNameVersion::COCO_BODY_UUNP_V4:
    case BodyNameVersion::COCO_BODY_UUNP_V6:
    case BodyNameVersion::COCO_BODY_UUNP_V6_8:
    case BodyNameVersion::COCO_BODY_UUNP_V6_9_AND_V6_95:
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
      // If not "2.13 to 2.22 - 2.30 - 2.31" or "2.40 to 2.42" but trying to use nevernude or underwear key, this is invalid
      if (aRelativeVariant > 0
          && !(aRelativeVersion >= 4 && aRelativeVersion <= 5) // "2.13 to 2.22 - 2.30 - 2.31" or "2.40 to 2.42"
      )
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
        case 7: // "3.04 - 3.05"
        case 8: // "3.10"
        case 9: // "3.11 to 3.13"
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
    case FeetNameVersion::BHUNP_3BBB_ADVANCED_3_10:
    case FeetNameVersion::BHUNP_3BBB_ADVANCED_3_11_TO_3_13:
      return FeetVariant::BHUNP_3BBB_ADVANCED;
    case FeetNameVersion::CBBE_3BA_3BBB_1_50:
    case FeetNameVersion::CBBE_3BA_3BBB_1_51_TO_1_55:
    case FeetNameVersion::CBBE_3BA_3BBB_2_02_TO_2_04:
    case FeetNameVersion::CBBE_3BA_3BBB_2_06_AND_2_11_AND_2_12:
    case FeetNameVersion::CBBE_3BA_3BBB_2_13_TO_2_18_AND_2_30_AND_2_31:
    case FeetNameVersion::CBBE_3BA_3BBB_2_40_TO_2_42:
      return FeetVariant::CBBE_3BA_3BBB;
    case FeetNameVersion::COCO_BODY_NORMAL_CBBE_V4:
    case FeetNameVersion::COCO_BODY_NORMAL_CBBE_V6:
    case FeetNameVersion::COCO_BODY_NORMAL_CBBE_V6_8:
    case FeetNameVersion::COCO_BODY_NORMAL_CBBE_V6_9_AND_V6_95:
      return FeetVariant::COCO_BODY_NORMAL_CBBE;
    case FeetNameVersion::COCO_BODY_NORMAL_UUNP_V3:
    case FeetNameVersion::COCO_BODY_NORMAL_UUNP_V4:
    case FeetNameVersion::COCO_BODY_NORMAL_UUNP_V6:
    case FeetNameVersion::COCO_BODY_NORMAL_UUNP_V6_8:
    case FeetNameVersion::COCO_BODY_NORMAL_UUNP_V6_9_AND_V6_95:
      return FeetVariant::COCO_BODY_NORMAL_UUNP;
    case FeetNameVersion::COCO_BODY_HIGH_HEELS_CBBE_V6:
    case FeetNameVersion::COCO_BODY_HIGH_HEELS_CBBE_V6_8:
    case FeetNameVersion::COCO_BODY_HIGH_HEELS_CBBE_V6_9_AND_V6_95:
      return FeetVariant::COCO_BODY_HIGH_HEELS_CBBE;
    case FeetNameVersion::COCO_BODY_HIGH_HEELS_UUNP_V6:
    case FeetNameVersion::COCO_BODY_HIGH_HEELS_UUNP_V6_8:
    case FeetNameVersion::COCO_BODY_HIGH_HEELS_UUNP_V6_9_AND_V6_95:
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
        case 7: // "3.04 - 3.05"
        case 8: // "3.10"
        case 9: // "3.11 to 3.13"
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
    case BodyNameVersion::CBBE_3BA_3BBB_2_40_TO_2_42:
    case BodyNameVersion::BHUNP_3BBB_2_20:
    case BodyNameVersion::BHUNP_3BBB_2_25:
    case BodyNameVersion::BHUNP_3BBB_2_30:
    case BodyNameVersion::BHUNP_3BBB_2_31:
    case BodyNameVersion::BHUNP_3BBB_2_35_TO_2_42:
    case BodyNameVersion::BHUNP_3BBB_3_00_TO_3_02:
    case BodyNameVersion::BHUNP_3BBB_3_03:
    case BodyNameVersion::BHUNP_3BBB_3_04:
    case BodyNameVersion::BHUNP_3BBB_3_10:
    case BodyNameVersion::BHUNP_3BBB_3_11_TO_3_13:
    case BodyNameVersion::BHUNP_3BBB_ADVANCED_2_20:
    case BodyNameVersion::BHUNP_3BBB_ADVANCED_2_25:
    case BodyNameVersion::BHUNP_3BBB_ADVANCED_2_30:
    case BodyNameVersion::BHUNP_3BBB_ADVANCED_2_31:
    case BodyNameVersion::BHUNP_3BBB_ADVANCED_2_35_TO_2_42:
    case BodyNameVersion::BHUNP_3BBB_ADVANCED_3_00_TO_3_02:
    case BodyNameVersion::BHUNP_3BBB_ADVANCED_3_03:
    case BodyNameVersion::BHUNP_3BBB_ADVANCED_3_04:
    case BodyNameVersion::BHUNP_3BBB_ADVANCED_3_10:
    case BodyNameVersion::BHUNP_3BBB_ADVANCED_3_11_TO_3_13:
    case BodyNameVersion::BHUNP_3BBB_ADVANCED_VER_2_2_20:
    case BodyNameVersion::BHUNP_3BBB_ADVANCED_VER_2_2_25:
    case BodyNameVersion::BHUNP_3BBB_ADVANCED_VER_2_2_30:
    case BodyNameVersion::BHUNP_3BBB_ADVANCED_VER_2_2_31:
    case BodyNameVersion::BHUNP_3BBB_ADVANCED_VER_2_2_35_TO_2_42:
    case BodyNameVersion::BHUNP_3BBB_ADVANCED_VER_2_3_00_TO_3_02:
    case BodyNameVersion::BHUNP_3BBB_ADVANCED_VER_2_3_03:
    case BodyNameVersion::BHUNP_3BBB_ADVANCED_VER_2_3_04:
    case BodyNameVersion::BHUNP_3BBB_ADVANCED_VER_2_3_10:
    case BodyNameVersion::BHUNP_3BBB_ADVANCED_VER_2_3_11_TO_3_13:
    case BodyNameVersion::BHUNP_BBP_2_20:
    case BodyNameVersion::BHUNP_BBP_2_25:
    case BodyNameVersion::BHUNP_BBP_2_30:
    case BodyNameVersion::BHUNP_BBP_2_31:
    case BodyNameVersion::BHUNP_BBP_2_35_TO_2_42:
    case BodyNameVersion::BHUNP_BBP_3_00_TO_3_02:
    case BodyNameVersion::BHUNP_BBP_3_03:
    case BodyNameVersion::BHUNP_BBP_3_04:
    case BodyNameVersion::BHUNP_BBP_3_10:
    case BodyNameVersion::BHUNP_BBP_3_11_TO_3_13:
    case BodyNameVersion::BHUNP_BBP_ADVANCED_2_20:
    case BodyNameVersion::BHUNP_BBP_ADVANCED_2_25:
    case BodyNameVersion::BHUNP_BBP_ADVANCED_2_30:
    case BodyNameVersion::BHUNP_BBP_ADVANCED_2_31:
    case BodyNameVersion::BHUNP_BBP_ADVANCED_2_35_TO_2_42:
    case BodyNameVersion::BHUNP_BBP_ADVANCED_3_00_TO_3_02:
    case BodyNameVersion::BHUNP_BBP_ADVANCED_3_03:
    case BodyNameVersion::BHUNP_BBP_ADVANCED_3_04:
    case BodyNameVersion::BHUNP_BBP_ADVANCED_3_10:
    case BodyNameVersion::BHUNP_BBP_ADVANCED_3_11_TO_3_13:
    case BodyNameVersion::BHUNP_TBBP_2_20:
    case BodyNameVersion::BHUNP_TBBP_2_25:
    case BodyNameVersion::BHUNP_TBBP_2_30:
    case BodyNameVersion::BHUNP_TBBP_2_31:
    case BodyNameVersion::BHUNP_TBBP_2_35_TO_2_42:
    case BodyNameVersion::BHUNP_TBBP_3_00_TO_3_02:
    case BodyNameVersion::BHUNP_TBBP_3_03:
    case BodyNameVersion::BHUNP_TBBP_3_04:
    case BodyNameVersion::BHUNP_TBBP_3_10:
    case BodyNameVersion::BHUNP_TBBP_3_11_TO_3_13:
    case BodyNameVersion::BHUNP_TBBP_ADVANCED_2_20:
    case BodyNameVersion::BHUNP_TBBP_ADVANCED_2_25:
    case BodyNameVersion::BHUNP_TBBP_ADVANCED_2_30:
    case BodyNameVersion::BHUNP_TBBP_ADVANCED_2_31:
    case BodyNameVersion::BHUNP_TBBP_ADVANCED_2_35_TO_2_42:
    case BodyNameVersion::BHUNP_TBBP_ADVANCED_3_00_TO_3_02:
    case BodyNameVersion::BHUNP_TBBP_ADVANCED_3_03:
    case BodyNameVersion::BHUNP_TBBP_ADVANCED_3_04:
    case BodyNameVersion::BHUNP_TBBP_ADVANCED_3_10:
    case BodyNameVersion::BHUNP_TBBP_ADVANCED_3_11_TO_3_13:
    case BodyNameVersion::CBBE_SMP_3BBB_1_2_0:
    case BodyNameVersion::COCO_BODY_CBBE_V4:
    case BodyNameVersion::COCO_BODY_CBBE_V6:
    case BodyNameVersion::COCO_BODY_CBBE_V6_8:
    case BodyNameVersion::COCO_BODY_CBBE_V6_9_AND_V6_95:
    case BodyNameVersion::COCO_BODY_UUNP_V3:
    case BodyNameVersion::COCO_BODY_UUNP_V4:
    case BodyNameVersion::COCO_BODY_UUNP_V6:
    case BodyNameVersion::COCO_BODY_UUNP_V6_8:
    case BodyNameVersion::COCO_BODY_UUNP_V6_9_AND_V6_95:
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
    case BodyNameVersion::CBBE_3BA_3BBB_NEVERNUDE_2_40_TO_2_42:
    case BodyNameVersion::CBBE_3BA_3BBB_UNDERWEAR_2_13_TO_2_18_AND_2_30_AND_2_31:
    case BodyNameVersion::CBBE_3BA_3BBB_UNDERWEAR_2_40_TO_2_42:
    {
      return 4; // Ignore "1.50", "1.51 to 1.55", "2.02 to 2.04" and "2.06 - 2.11 - 2.12"
    }
    case BodyNameVersion::BHUNP_3BBB_ADVANCED_VER_3_3_00_TO_3_02:
    case BodyNameVersion::BHUNP_3BBB_ADVANCED_VER_3_3_03:
    case BodyNameVersion::BHUNP_3BBB_ADVANCED_VER_3_3_04:
    case BodyNameVersion::BHUNP_3BBB_ADVANCED_VER_3_3_10:
    case BodyNameVersion::BHUNP_3BBB_ADVANCED_VER_3_3_11_TO_3_13:
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
    case BodyNameVersion::BHUNP_3BBB_ADVANCED_VER_2_NEVERNUDE_3_10:
    case BodyNameVersion::BHUNP_3BBB_ADVANCED_VER_2_NEVERNUDE_3_11_TO_3_13:
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
          || (aRelativeVersion >= 5 && aRelativeVersion <= 9) // Only for "3.00 to 3.02", "3.03", "3.04 - 3.05", "3.10" and "3.11 to 3.13"
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

QString DataLists::GetQRCPathForResource(const BodyNameVersion aBodyNameVersion,
                                         const FeetNameVersion aFeetNameVersion,
                                         const MeshPartType aResourceType)
{
  QString lResourceType;
  QString lResourceName;
  QString lResourceVersion;

  switch (aResourceType)
  {
    case MeshPartType::UNKNOWN:
    {
      // Do not do anything
      break;
    }
    case MeshPartType::BODY:
    {
      lResourceType = "body";
      lResourceName = DataLists::GetQRCResourceName(DataLists::GetVariant(aBodyNameVersion));
      lResourceVersion = DataLists::GetVersionString(aBodyNameVersion);
      break;
    }
    case MeshPartType::FEET:
    {
      lResourceType = "feet";
      lResourceName = DataLists::GetQRCResourceName(DataLists::GetVariant(aBodyNameVersion), aFeetNameVersion);
      lResourceVersion = DataLists::GetVersionString(aBodyNameVersion, aFeetNameVersion);
      break;
    }
    case MeshPartType::HANDS:
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
          lResourceName = QStringLiteral("cbbe 3bbb 3ba");
          break;
        }
        case BodyName::BHUNP_UUNP_NEXT_GENERATION:
        {
          const auto lBodyVariant{DataLists::GetVariant(aBodyNameVersion)};
          if (lBodyVariant == BodyVariant::BHUNP_3BBB_ADVANCED_VER_3)
          {
            lResourceName = QStringLiteral("bhunp ver 3");
          }
          else
          {
            lResourceName = QStringLiteral("bhunp");
          }
          break;
        }
        case BodyName::CBBE_SMP_3BBB:
        {
          lResourceName = QStringLiteral("cbbe");
          lResourceVersion = QStringLiteral("1.6.1"); // Default CBBE hands
          break;
        }
        case BodyName::COCO_BODY_CBBE:
        {
          lResourceName = QStringLiteral("coco body cbbe");
          break;
        }
        case BodyName::COCO_BODY_UUNP:
        {
          lResourceName = QStringLiteral("coco body uunp");
          break;
        }
        case BodyName::MIMIR_EBONIC_BODY:
        {
          lResourceName = QStringLiteral("mimir ebonic body");
          break;
        }
        case BodyName::BHUNP_LITE_ASDASDF:
        {
          lResourceName = QStringLiteral("asdasdf");
          break;
        }
      }

      break;
    }
    case MeshPartType::BEAST_HANDS:
    {
      // For the beast hands, use the default CBBE v.1.6.1 beast hands
      if (Utils::IsCBBEBasedBody(aBodyNameVersion))
      {
        lResourceType = "beast_hands";

        const auto lBodyName{DataLists::GetName(aBodyNameVersion)};
        const auto lVersionIndex{DataLists::GetVersionIndex(aBodyNameVersion)};

        // Only for CBBE 3BA 3BBB "2.40 to 2.42"
        if (lBodyName == BodyName::CBBE_3BA_3BBB && lVersionIndex == 5)
        {
          lResourceName = DataLists::GetQRCResourceName(DataLists::GetVariant(aBodyNameVersion), aFeetNameVersion);
          lResourceVersion = DataLists::GetVersionString(aBodyNameVersion, aFeetNameVersion);
        }
        else
        {
          lResourceName = QStringLiteral("cbbe");
          lResourceVersion = QStringLiteral("1.6.1");
        }

        break;
      }

      // UNP-based bodies do not have beast hands compatibility
      return QString();
    }
  }

  return QStringLiteral(":/presets/%1/%2/%3").arg(lResourceType, lResourceName, lResourceVersion);
}

std::pair<int, int> DataLists::GetSplittedNameVersionFromBodyVersionCompatibility(const int aBodyVersion)
{
  switch (aBodyVersion)
  {
    case 0: // CBBE_3BBB_3BA_1_50
    case 1: // CBBE_3BBB_3BA_1_51_TO_1_55
      return std::make_pair(static_cast<int>(0), static_cast<int>(aBodyVersion));
    case 17: // CBBE_3BBB_3BA_2_02_TO_2_04
      return std::make_pair(static_cast<int>(0), 2);
    case 18: // CBBE_3BBB_3BA_2_06
      return std::make_pair(static_cast<int>(0), 3);
    case 2: // CBBE_SMP_3BBB_1_2_0
      return std::make_pair(static_cast<int>(1), 0);
    case 3: // BHUNP_3BBB_2_20:
    case 4: // BHUNP_3BBB_ADVANCED_2_20:
    case 5: // BHUNP_3BBB_ADVANCED_VER_2_2_20:
    case 6: // BHUNP_BBP_2_20:
    case 7: // BHUNP_BBP_ADVANCED_2_20:
    case 8: // BHUNP_TBBP_2_20:
    case 9: // BHUNP_TBBP_ADVANCED_2_20:
      return std::make_pair(static_cast<int>(aBodyVersion) - 1, 0);
    case 10: // BHUNP_3BBB_2_25:
    case 11: // BHUNP_3BBB_ADVANCED_2_25:
    case 12: // BHUNP_3BBB_ADVANCED_VER_2_2_25:
    case 13: // BHUNP_BBP_2_25:
    case 14: // BHUNP_BBP_ADVANCED_2_25:
    case 15: // BHUNP_TBBP_2_25:
    case 16: // BHUNP_TBBP_ADVANCED_2_25:
      return std::make_pair(static_cast<int>(aBodyVersion) - 8, 1);
    case 19: // BHUNP_3BBB_2_30:
    case 20: // BHUNP_3BBB_ADVANCED_2_30:
    case 21: // BHUNP_3BBB_ADVANCED_VER_2_2_30:
    case 22: // BHUNP_BBP_2_30:
    case 23: // BHUNP_BBP_ADVANCED_2_30:
    case 24: // BHUNP_TBBP_2_30:
    case 25: // BHUNP_TBBP_ADVANCED_2_30:
      return std::make_pair(static_cast<int>(aBodyVersion) - 17, 2);
    case 26: // BHUNP_3BBB_ADVANCED_VER_2_NEVERNUDE_2_25
    case 27: // BHUNP_3BBB_ADVANCED_VER_2_NEVERNUDE_2_30
      return std::make_pair(static_cast<int>(9), static_cast<int>(aBodyVersion) - 26);
    case 28: // BHUNP_3BBB_2_31:
    case 29: // BHUNP_3BBB_ADVANCED_2_31:
    case 30: // BHUNP_3BBB_ADVANCED_VER_2_2_31:
    case 31: // BHUNP_BBP_2_31:
    case 32: // BHUNP_BBP_ADVANCED_2_31:
    case 33: // BHUNP_TBBP_2_31:
    case 34: // BHUNP_TBBP_ADVANCED_2_31:
      return std::make_pair(static_cast<int>(aBodyVersion) - 26, 3);
    case 35: // BHUNP_3BBB_2_35_TO_2_39:
    case 36: // BHUNP_3BBB_ADVANCED_2_35_TO_2_39:
    case 37: // BHUNP_3BBB_ADVANCED_VER_2_2_35_TO_2_39:
    case 38: // BHUNP_BBP_2_35_TO_2_39:
    case 39: // BHUNP_BBP_ADVANCED_2_35_TO_2_39:
    case 40: // BHUNP_TBBP_2_35_TO_2_39:
    case 41: // BHUNP_TBBP_ADVANCED_2_35_TO_2_39:
      return std::make_pair(static_cast<int>(aBodyVersion) - 33, 4);
    case 42: // MIMIR_EBONIC_BODY_1_2:
    case 43: // MIMIR_EBONIC_BODY_1_2_FOOT_SEAMS_FIX:
      return std::make_pair(static_cast<int>(BodyName::MIMIR_EBONIC_BODY), static_cast<int>(aBodyVersion) - 42);
  }

  return std::make_pair(-1, -1);
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
                      QStringLiteral("femalebody_1"),
                      QStringLiteral("femalebody_1_s"),
                      QStringLiteral("femalebody_1_sk"),
                      QStringLiteral("femalebody_1_msn"),
                      // Body extra
                      QStringLiteral("femalebody_etc_1"),
                      QStringLiteral("femalebody_etc_1_s"),
                      QStringLiteral("femalebody_etc_1_sk"),
                      QStringLiteral("femalebody_etc_1_msn"),
                      QStringLiteral("femalebody_etc_v2_1"),
                      QStringLiteral("femalebody_etc_v2_1_s"),
                      QStringLiteral("femalebody_etc_v2_1_sk"),
                      QStringLiteral("femalebody_etc_v2_1_msn"),
                      // Hands
                      QStringLiteral("femalehands_1"),
                      QStringLiteral("femalehands_1_s"),
                      QStringLiteral("femalehands_1_sk"),
                      QStringLiteral("femalehands_1_msn"),
                      // Head
                      QStringLiteral("femalehead"),
                      QStringLiteral("femalehead_s"),
                      QStringLiteral("femalehead_sk"),
                      QStringLiteral("femalehead_msn"),
                      QStringLiteral("femaleheadvampire"),
                      QStringLiteral("femaleheadvampire_msn"),
                      QStringLiteral("femaleheaddetail_frekles"),
                      QStringLiteral("femaleheaddetail_rough"),
                      // Mouth
                      QStringLiteral("mouthhuman"),
                      QStringLiteral("mouthhuman_n")});
}

QString DataLists::GetBodySliderValue(const BodyNameVersion& aBodyNameVersion)
{
  const auto lBodyVariant{DataLists::GetVariant(aBodyNameVersion)};
  const auto lVersionIndex{DataLists::GetVersionIndex(aBodyNameVersion)};

  switch (lBodyVariant)
  {
    case BodyVariant::CBBE_3BA_3BBB:
    case BodyVariant::MIMIR_EBONIC_BODY:
      return QStringLiteral("CBBE 3BBB Body Amazing");
    case BodyVariant::CBBE_3BA_3BBB_NEVERNUDE:
      return QStringLiteral("CBBE 3BBB Amazing NeverNude");
    case BodyVariant::CBBE_3BA_3BBB_UNDERWEAR:
      return QStringLiteral("CBBE 3BBB Amazing Underwear");
    case BodyVariant::BHUNP_3BBB:
      return QStringLiteral("BHUNP 3BBB");
    case BodyVariant::BHUNP_3BBB_ADVANCED:
      return QStringLiteral("BHUNP 3BBB Advanced");
    case BodyVariant::BHUNP_3BBB_ADVANCED_VER_2:
      return QStringLiteral("BHUNP 3BBB Advanced Ver 2");
    case BodyVariant::BHUNP_3BBB_ADVANCED_VER_3:
      return QStringLiteral("BHUNP 3BBB Advanced Ver 3");
    case BodyVariant::BHUNP_BBP:
      return QStringLiteral("BHUNP BBP");
    case BodyVariant::BHUNP_BBP_ADVANCED:
      return QStringLiteral("BHUNP BBP Advanced");
    case BodyVariant::BHUNP_TBBP:
      return QStringLiteral("BHUNP TBBP");
    case BodyVariant::BHUNP_TBBP_ADVANCED:
      return QStringLiteral("BHUNP TBBP Advanced");
    case BodyVariant::BHUNP_3BBB_ADVANCED_VER_2_NEVERNUDE:
      return QStringLiteral("BHUNP 3BBB Advanced Ver 2 Nevernude");
    case BodyVariant::CBBE_SMP_3BBB:
      return QStringLiteral("CBBE Body SMP (3BBB)");
    case BodyVariant::COCO_BODY_CBBE:
      switch (lVersionIndex)
      {
        case 0:
          return QStringLiteral("[COCO]3Bsmp_BodyV4_A");
        case 1:
          return QStringLiteral("[COCO 3BBB V6]Body_A");
      }
      return QString();
    case BodyVariant::COCO_BODY_UUNP:
      switch (lVersionIndex)
      {
        case 0:
          return QStringLiteral("[COCO]body_B_v3");
        case 1:
          return QStringLiteral("[COCO]3Bsmp_BodyV4_B");
        case 2:
          return QStringLiteral("[COCO 3BBB V6]Body_B");
      }
      return QString();
    case BodyVariant::ASDASDF_3BBB:
      return QStringLiteral("ASDASDF-BHUNP 3BBB");
    case BodyVariant::ASDASDF_3BBB_NO_THIGH_JIGGLE:
      return QStringLiteral("ASDASDF-BHUNP 3BBB - No Thigh Jiggle");
    case BodyVariant::ASDASDF_BBP:
      return QStringLiteral("ASDASDF-BHUNP BBP");
    case BodyVariant::ASDASDF_BBP_NO_THIGH_JIGGLE:
      return QStringLiteral("ASDASDF-BHUNP BBP - No Thigh Jiggle");
    case BodyVariant::ASDASDF_TBBP:
      return QStringLiteral("ASDASDF-BHUNP TBBP");
    case BodyVariant::ASDASDF_TBBP_NO_THIGH_JIGGLE:
      return QStringLiteral("ASDASDF-BHUNP TBBP - No Thigh Jiggle");
    case BodyVariant::ASDASDF_ALTERBODY_3BBB:
      return QStringLiteral("ASDASDF-BHUNP_AlterBody 3BBB");
    case BodyVariant::ASDASDF_ALTERBODY_3BBB_NO_THIGH_JIGGLE:
      return QStringLiteral("ASDASDF-BHUNP_AlterBody 3BBB - No Thigh Jiggle");
    case BodyVariant::ASDASDF_ALTERBODY_BBP:
      return QStringLiteral("ASDASDF-BHUNP_AlterBody BBP");
    case BodyVariant::ASDASDF_ALTERBODY_BBP_NO_THIGH_JIGGLE:
      return QStringLiteral("ASDASDF-BHUNP_AlterBody BBP - No Thigh Jiggle");
    case BodyVariant::ASDASDF_ALTERBODY_TBBP:
      return QStringLiteral("ASDASDF-BHUNP_AlterBody TBBP");
    case BodyVariant::ASDASDF_ALTERBODY_TBBP_NO_THIGH_JIGGLE:
      return QStringLiteral("ASDASDF-BHUNP_AlterBody TBBP - No Thigh Jiggle");
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
      return QStringLiteral("CBBE Feet");
    case FeetVariant::BHUNP_3BBB_ADVANCED:
      return QStringLiteral("BHUNP 3BBB Advanced Feet");
    case FeetVariant::CBBE_3BA_3BBB:
      return QStringLiteral("CBBE 3BBB Feet");
    case FeetVariant::COCO_BODY_NORMAL_CBBE:
      switch (lVersionIndex)
      {
        case 0: // "V4"
          return QStringLiteral("[COCO]3Bsmp_FeetV4_A");
        case 1: // "V6"
        case 2: // "V6.8"
        case 3: // "V6.9 - V6.95"
          return QStringLiteral("[COCO 3BBB V6]Feet_A");
      }
      return QString();
    case FeetVariant::COCO_BODY_NORMAL_UUNP:
      switch (lVersionIndex)
      {
        case 0: // "V3"
          return QStringLiteral("[COCO]bodyFeets_B_v3");
        case 1: // "V4"
          return QStringLiteral("[COCO]3Bsmp_FeetV4_B");
        case 2: // "V6"
        case 3: // "V6.8"
        case 4: // "V6.9 - V6.95"
          return QStringLiteral("[COCO 3BBB V6]Feet_B");
      }
      return QString();
    case FeetVariant::COCO_BODY_HIGH_HEELS_CBBE:
      switch (lVersionIndex)
      {
        case 0: // "V4"
        case 1: // "V6"
        case 2: // "V6.8"
          return QStringLiteral("[COCO]CBBEV6_HH100");
        case 3: // "V6.9 - V6.95"
          return QStringLiteral("[COCO]CBBEV6_Heelfeets");
      }
      return QString();
    case FeetVariant::COCO_BODY_HIGH_HEELS_UUNP:
      switch (lVersionIndex)
      {
        case 0: // "V3"
        case 1: // "V4"
        case 2: // "V6"
        case 3: // "V6.8"
          return QStringLiteral("[COCO]UUNPV6_HH100");
        case 4: // "V6.9 - V6.95"
          return QStringLiteral("[COCO]UUNPV6_Heelfeets");
      }
      return QString();
    case FeetVariant::MIMIR_EBONIC_BODY:
      return QStringLiteral("CBBE 3BBB Feet");
    case FeetVariant::MORE_SLIDERS_FOR_FEET_NORMAL_CBBE:
      return QStringLiteral("CBBE MSF Feet");
    case FeetVariant::MORE_SLIDERS_FOR_FEET_NORMAL_BHUNP:
      return QStringLiteral("BHUNP Feet MSF");
    case FeetVariant::MORE_SLIDERS_FOR_FEET_HIGH_HEELS_CBBE:
      return QStringLiteral("CBBE MSF Feet High Heel");
    case FeetVariant::MORE_SLIDERS_FOR_FEET_HIGH_HEELS_BHUNP:
      return QStringLiteral("BHUNP Feet High Heel MSF");
    case FeetVariant::HG_FEET:
      return QStringLiteral("HGFeet UUNP");
    case FeetVariant::KHRYSAMERE_HG_FEET:
      switch (lVersionIndex)
      {
        case 0:
          return QStringLiteral("[Khrysamere] HG Feet (BHUNP)");
        case 1:
          return QStringLiteral("[Khrysamere] K Feet");
        case 2:
          return QStringLiteral("[Khrysamere] K Feet");
        case 3:
          return QStringLiteral("[Khrysamere] K Feet v5.0");
        case 4:
          return QStringLiteral("[Khrysamere] K Feet");
      }
      return QString();
    case FeetVariant::ASDASDF:
      return QStringLiteral("ASDASDF-BHUNP Feet");
    case FeetVariant::_INVALID_VALUE:
      return QString();
  }

  return QString();
}

QString DataLists::GetHandsSliderValue(const BodyNameVersion& aBodyNameVersion, const bool aMustUseBeastHands)
{
  const auto lBodyName{DataLists::GetName(aBodyNameVersion)};
  const auto lVersionIndex{DataLists::GetVersionIndex(aBodyNameVersion)};

  // Beast hands
  if (Utils::IsCBBEBasedBody(aBodyNameVersion) && aMustUseBeastHands)
  {
    // Only for CBBE 3BA 3BBB "2.40 to 2.42"
    if (lBodyName == BodyName::CBBE_3BA_3BBB && lVersionIndex == 5)
      return QStringLiteral("CBBE 3BBB Hands Beast");

    return QStringLiteral("CBBE Hands Beast");
  }

  switch (lBodyName)
  {
    case BodyName::CBBE_3BA_3BBB:
    case BodyName::MIMIR_EBONIC_BODY:
    {
      return QStringLiteral("CBBE 3BBB Hands");
    }
    case BodyName::BHUNP_UUNP_NEXT_GENERATION:
    {
      // TODO: Remove this workaround once the user can choose hands meshes
      const auto lBodyVariant{DataLists::GetVariant(aBodyNameVersion)};
      if (lBodyVariant == BodyVariant::BHUNP_3BBB_ADVANCED_VER_3
          && (lVersionIndex >= 5 && lVersionIndex <= 9) // Only for "3.00 to 3.02", "3.03", "3.04 - 3.05", "3.10" and "3.11 to 3.13"
      )
      {
        return QStringLiteral("BHUNP 3BBB Advanced Hands Ver 3");
      }

      return QStringLiteral("BHUNP 3BBB Advanced Hands");
    }
    case BodyName::CBBE_SMP_3BBB:
    {
      return QStringLiteral("CBBE Hands");
    }
    case BodyName::COCO_BODY_CBBE:
    {
      switch (lVersionIndex)
      {
        case 0:
          return QStringLiteral("[COCO]3Bsmp_HandV4_A");
        case 1:
          return QStringLiteral("[COCO 3BBB V6]Hands_A");
      }

      break;
    }
    case BodyName::COCO_BODY_UUNP:
    {
      switch (lVersionIndex)
      {
        case 0:
          return QStringLiteral("[COCO]bodyHands_B_v3");
        case 1:
          return QStringLiteral("[COCO]3Bsmp_HandV4_B");
        case 2:
          return QStringLiteral("[COCO 3BBB V6]Hands_B");
      }

      break;
    }
    case BodyName::BHUNP_LITE_ASDASDF:
    {
      return QStringLiteral("ASDASDF-BHUNP Hands");
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
  return QStringList{QObject::tr("Minimized"),
                     QObject::tr("Windowed"),
                     QObject::tr("Maximized")};
}

QStringList DataLists::GetDialogOpeningModes()
{
  return QStringList{QObject::tr("Windowed"),
                     QObject::tr("Maximized")};
}

QStringList DataLists::GetAppThemes()
{
  return QStringList{QStringLiteral("Default OS theme"),
                     QStringLiteral("Mitsuriou's Black Theme"),
                     QStringLiteral("Mitsuriou's Dark Theme"),
                     QStringLiteral("Mitsuriou's Light Theme"),
                     QStringLiteral("Alexhuszagh's Breeze Dark"),
                     QStringLiteral("Alexhuszagh's Breeze Light"),
                     QStringLiteral("Paper Dark by 6788"),
                     QStringLiteral("Paper Light by 6788"),
                     QStringLiteral("Paper Black Mono by 6788"),
                     QStringLiteral("Paper White Mono by 6788")};
}

QStringList DataLists::GetLanguages()
{
  QStringList lLanguages{QStringLiteral("English"),
                         QStringLiteral("Français"),
                         QStringLiteral("繁体中文(台湾) - translation by Sephirothryan")};

  return lLanguages;
}

QStringList DataLists::GetAssistedConversionActions()
{
  return QStringList{QObject::tr("[ignore]"),
                     QObject::tr("Body mesh"),
                     QObject::tr("Feet mesh"),
                     QObject::tr("Hands mesh"),
                     QObject::tr("Skeleton mesh")};
}

QStringList DataLists::GetSliderSetsImporterActions()
{
  return QStringList{QObject::tr("[ignore]"),
                     QObject::tr("Body mesh"),
                     QObject::tr("Feet mesh"),
                     QObject::tr("Hands mesh"),
                     QObject::tr("Beast hands mesh")};
}

QStringList DataLists::GetLastPathsKeys()
{
  // Note: keep this list sorted since indexes are used to bind some Settings actions
  return QStringList{QStringLiteral("general"),
                     QStringLiteral("mainWindowOutput"),
                     QStringLiteral("batchConversionInput"),
                     QStringLiteral("batchConversionOutput"),
                     QStringLiteral("assistedConversionInput"),
                     QStringLiteral("retargetingToolInput"),
                     QStringLiteral("retargetingToolOutput"),
                     QStringLiteral("texturesAssistantInput"),
                     QStringLiteral("texturesAssistantOutput"),
                     QStringLiteral("sliderSetsImporterInput"),
                     QStringLiteral("lastInjectedOSPFile"),
                     QStringLiteral("lastLoadedProject"),
                     QStringLiteral("lastSavedProject")};
}
