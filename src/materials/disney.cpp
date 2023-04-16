#include "disney.h"
#include "useful.h"
#include "transform.h"

Color Disney::EvaluateDiffuse(const PrincipledParameters &parameters, const Vector3f &wi, const Vector3f &wo, const Vector3f &wh, const Normal3f &n){
    Color fLambert = parameters.baseColor / Pi; // this could change later

    double cosL = AbsDot(wi, n);
    double cosV = AbsDot(wo, n);
    double cosD = AbsDot(wi, wh);

    double fL = pow(1 - cosL, 5);

    double fV = pow(1 - cosV, 5);

    double rR = 2 * parameters.roughness * cosD * cosD;

    Color fRetro = (parameters.baseColor / Pi) * rR * (fL + fV + fL * fV * (rR - 1));

    Color fDiffuse = fLambert * (1 - 0.5 * fL) * (1 - 0.5 * fV) + fRetro;

    return fDiffuse;
}


double Disney::GTR2(const PrincipledParameters &parameters, const Vector3f &wh, const Normal3f &n){
    // Trowbridge-Reitz distribution
    double alpha = parameters.roughness * parameters.roughness;
    return (alpha * alpha) / (Pi * pow(pow(AbsDot(n, wh), 2) * (alpha * alpha - 1) + 1, 2));
}

double heavyside(double a){
    if(a > 0.) return 1.;
    return 0.;
}

double lambda(const Vector3f &w, const Normal3f &n, double alpha){
    double tanTheta = (Cross(w, (Vector3f)n)).Norm() / Dot(w, n);
    return (-1 + std::sqrt(1 + pow(alpha * tanTheta, 2))) / 2;
}

double Disney::G1(const Vector3f &w, const Vector3f &wh,const Normal3f &n, double alpha){
    return heavyside(Dot(w, wh)) / (1 + lambda(w, n, alpha));
}

double Disney::SmithGGX(const PrincipledParameters &parameters, const Vector3f &wi, const Vector3f &wo, const Vector3f &wh, const Normal3f &n){
    double alpha = parameters.roughness * parameters.roughness;
    return G1(wi, wh, n, alpha) * G1(wo, wh, n, alpha);

}

Color Disney::SchlickFresnel(const PrincipledParameters &parameters, const Vector3f &wh, const Vector3f &wo, const Vector3f &wi){
    double f0 = pow((parameters.IOR - 1) / (parameters.IOR + 1), 2); 
    Color F0 = f0 * (1 - parameters.metallic) + parameters.baseColor * (parameters.metallic);
   double dFresnel = f0 + (1 - f0) * pow((1 - AbsDot(wh, wo)), 5);
    Color mFresnel = F0 + (1 + (-F0)) * pow((1 - AbsDot(wh, wi)), 5);
    return dFresnel * (1 - parameters.metallic) + mFresnel * (parameters.metallic);
}

Color Disney::EvaluateSpecBRDF(const PrincipledParameters &parameters, const Vector3f &wi, const Vector3f &wo, const Vector3f &wh, const Normal3f &n, double *pdf){
   double dotNL = Dot(wi, n);
   double dotNV = Dot(wo, n);
    if(dotNL <= 0 || dotNV <= 0){ // perhaps not necessary
        return Color(0,0,0);
    }

   double d = GTR2(parameters, wh, n);
   double g = SmithGGX(parameters, wi, wo, wh, n);
    Color f = SchlickFresnel(parameters, wh, wo, wi);

    *pdf = d * G1(wo, wh, Normal3f(0, 1, 0), pow(parameters.roughness, 2)) / (4 * std::abs(dotNV)); // the other term cancels out surely

    return (d * g * f) / (4. * dotNL * dotNV);
}

double FrDielectric(double cosThetaI, double etaI, double etaT){
    cosThetaI = clamp(cosThetaI, -1, 1);
    bool entering = cosThetaI > 0.f;
    if (!entering) {
        std::swap(etaI, etaT);
        cosThetaI = std::abs(cosThetaI);
    }

    // Compute _cosThetaT_ using Snell's law
    double sinThetaI = std::sqrt(std::max(0., 1 - cosThetaI * cosThetaI));
    double sinThetaT = etaI / etaT * sinThetaI;

    // Handle total internal reflection
    if (sinThetaT >= 1) return 1;
    double cosThetaT = std::sqrt(std::max(0., 1 - sinThetaT * sinThetaT));
    double Rparl = ((etaT * cosThetaI) - (etaI * cosThetaT)) /
                  ((etaT * cosThetaI) + (etaI * cosThetaT));
    double Rperp = ((etaI * cosThetaI) - (etaT * cosThetaT)) /
                  ((etaI * cosThetaI) + (etaT * cosThetaT));
    return (Rparl * Rparl + Rperp * Rperp) / 2;
}

Color Disney::EvaluateSpecTransmission(const PrincipledParameters &parameters, const Vector3f &wi, const Vector3f &wo, const Vector3f &wt, const Normal3f &n, double *pdf){
    double absDotNL = AbsDot(wi, n);
    double absDotNV = AbsDot(wo, n);
    double dotHL = Dot(wt, wi);
    double dotHV = Dot(wt, wo);
    double absDotHL = std::abs(dotHL);
    double absDotHV = std::abs(dotHV);

    double d = GTR2(parameters, wt, n);
    double g = SmithGGX(parameters, wi, wo, wt, n);
    double f = FrDielectric(dotHV, 1., 1. / parameters.IOR);

    double c = (absDotHL * absDotHV) / (absDotNL * absDotNV);
    double t = (pow(parameters.IOR, 2) / pow(dotHL + parameters.IOR * dotHV, 2));
    double relativeIOR = wo.y > 0 ? 1. / parameters.IOR : parameters.IOR;

    *pdf = d * G1(wo, wt, Normal3f(0, 1, 0), pow(parameters.roughness, 2)) * absDotHV / (absDotNV * pow(dotHL + relativeIOR * dotHV, 2));
    return parameters.baseColor * c * t * (1. - f) * g * d;
}

void Disney::CalculateLobePDF(const PrincipledParameters &parameters, double &pSpecular, double &pDiffuse, double &pSpecTrans){
    double dielectric = (1. - parameters.specTrans) * (1. - parameters.metallic);
    double specWeight = parameters.metallic + dielectric;
    double transWeight = (1. - parameters.metallic) * parameters.specTrans;
    double norm = 1. / (specWeight + transWeight + dielectric);

    pSpecular  = specWeight * norm;
    pSpecTrans = transWeight * norm;
    pDiffuse   = dielectric * norm;
}

Vector3f SampleGGX(const Vector3f &wo, double alpha, double rand1, double rand2){
    // reminder, we need to be in tangent space here plz thanks (with Y-axis as up)
    Vector3f v = Normalize(Vector3f(wo.x * alpha, wo.y, wo.z * alpha));
    double lensq = v.x * v.x + v.z * v.z;
    Vector3f t1 = (lensq > 0) ? Vector3f(-v.z, 0, v.x) / sqrt(lensq) : Vector3f(1, 0, 0);
    Vector3f t2;
    if(v.y < 0){
        t2 = Cross(t1, v);
    }else{
        t2 = Cross(v, t1);
    }
    double a = 1 / (1 + v.y);
    double r = std::sqrt(rand1);
    double phi = (rand2 < a) ? (rand2 / a) * Pi : Pi + (rand2 - a) / (1 - a) * Pi;

    double p1 = r * std::cos(phi);
    double p2 = r * std::sin(phi) * ((rand2 < a) ? 1. : v.y);

    Vector3f wh = p1 * t1 + p2 * t2 + std::sqrt(std::max(0., 1. - p1 * p1 - p2 * p2)) * v;

    return Normalize(Vector3f(wh.x * alpha, std::max(0., wh.y), wh.z * alpha));
}

Vector3f reflect(const Vector3f &v, const Vector3f &wh){
    return -v - 2 * Dot(wh, -v) * wh;
}

bool transmit(const Vector3f &v, const Vector3f &wh, double relativeIOR, Vector3f &wi){
    double cosThetaI = Dot(v, wh);
    double sin2ThetaI = std::max(0., (1 - cosThetaI * cosThetaI));
    double sin2ThetaT = relativeIOR * relativeIOR * sin2ThetaI;
    if(sin2ThetaT > 1) return false;
    double cosThetaT = std::sqrt(1 - sin2ThetaT);
    wi = relativeIOR * -v + (relativeIOR * cosThetaI - cosThetaT) * wh;
    return true;
}

BSDFSample Disney::SampleSpecBRDF(const PrincipledParameters &parameters, const Vector3f &wo){
    // wo should be normalized in tangent space plz thanks
    BSDFSample sample;
    double alpha = parameters.roughness * parameters.roughness;
    double rand1 = random_double();
    double rand2 = random_double();
    Vector3f wh = SampleGGX(wo, alpha, rand1, rand2);


    // std::cout << "Outgoing: [" << wo.x << ", " << wo.y << ", " << wo.z << "]" << std::endl;
    // std::cout << "Half: [" << wh.x << ", " << wh.y << ", " << wh.z << "]" << std::endl;

    Vector3f wi = Normalize(reflect(wo, wh));

    // std::cout << "Incoming: [" << wi.x << ", " << wi.y << ", " << wi.z << "]" << std::endl;

    if(Dot(wi, Normal3f(0, 1, 0)) <= 0.){
        sample.reflectance = Color(0,0,0);
        sample.isBlack = true;
        // std::cout << "it dies" << std::endl;
    }else{
        Color fresnel = SchlickFresnel(parameters, wh, wo, wi);
        double g = G1(wo, wh, Normal3f(0, 1, 0), alpha);
        // std::cout << "fresnel: " << fresnel << std::endl;
        // std::cout << "g: " << g << std::endl;

    sample.reflectance = g * fresnel;
    }
    double d = GTR2(parameters, wh, Normal3f(0, 1, 0));
    sample.pdf = d * G1(wo, wh, Normal3f(0, 1, 0), pow(parameters.roughness, 2)) / (4 * AbsDot(wo, Normal3f(0, 1, 0)));
    
    sample.wi = wi;
    return sample;
}

Vector3f SampleCosineWeightedHemisphere(double r0, double r1){
    Point2f p = randomInCircle(Point2f(r0, r1));

    return Vector3f(p.x, std::max(0., std::sqrt(1 - p.x * p.x - p.y * p.y)), p.y);
}

BSDFSample Disney::SampleDiffuse(const PrincipledParameters &parameters, const Vector3f &wo){
    BSDFSample sample;
    double rand1 = random_double();
    double rand2 = random_double();
    Vector3f wi = SampleCosineWeightedHemisphere(rand1, rand2);
    Vector3f wh = Normalize(wi + wo);
    Color diffuse = EvaluateDiffuse(parameters, wi, wo, wh, Normal3f(0, 1, 0));
    sample.reflectance = diffuse;
    sample.wi = wi;
    sample.pdf = wi.y;
    return sample;
}

BSDFSample Disney::SampleSpecTrans(const PrincipledParameters &parameters, const Vector3f &wo){
    BSDFSample sample;
    if(wo.y == 0.0){
        sample.reflectance = Color(0, 0, 0);
        sample.wi = Vector3f(0, 0, 0);
        sample.isBlack = true;
        return sample;
    }
    double alpha = parameters.roughness * parameters.roughness;
    double rand1 = random_double();
    double rand2 = random_double();
    Vector3f wh = SampleGGX(wo, alpha, rand1, rand2);

    double dotHV = Dot(wo, wh);
    if(wo.y < 0.){
        dotHV = -dotHV;
    }
    

    double F = FrDielectric(dotHV, 1., parameters.IOR);

    double g = G1(wo, wh, Normal3f(0, 1, 0), alpha);

    Vector3f wi;

    if(random_double() <= F){
        wi = Normalize(reflect(wo, wh));
        sample.reflectance = Color(g, g, g);
        sample.pdf = F / (4 * std::abs(dotHV));
    }else{
        double relativeIOR = wo.y > 0 ? 1. / parameters.IOR : parameters.IOR;
        if(transmit(wo, wh, relativeIOR, wi)){
            wi = Normalize(wi);
            
            sample.reflectance = Color(g, g, g);
            double dotLH = AbsDot(wi, wh);
            sample.pdf = (1 - F) * pow(dotLH + relativeIOR * dotHV, 2) / dotLH;
        }else{
            std::cout << "wo:" << wo << std::endl;
            std::cout << "wh:" << wh << std::endl;
            std::cout << "F: " << F << std::endl;
            std::cout << "cosTheta: " << dotHV << std::endl;
            std::cout << "relativeIOR: " << relativeIOR << std::endl;
            std::cout << "bro wtf" << std::endl << std::endl;
            wi = Normalize(reflect(wo, wh));
            sample.pdf = 0; // cuz why not
            sample.reflectance = Color(g, g, g);
        }
    }
    double d = GTR2(parameters, wh, Normal3f(0, 1, 0));
    sample.pdf *= d * G1(wo, wh, Normal3f(0, 1, 0), pow(parameters.roughness, 2)) * std::abs(dotHV) / AbsDot(wo, Normal3f(0, 1, 0));
    sample.wi = wi;
    return sample;

}

BSDFSample Disney::SampleDisney(const SurfaceInteraction &isect, const Vector3f &v){
    PrincipledParameters parameters = isect.parameters;
     // std::cout << "Normal World: [" << isect.n.x << ", " << isect.n.y << ", " << isect.n.z << "]" << std::endl;

    Transform spaceToTangent = ToSpace((Vector3f)isect.n);
    Transform tangentToSpace = Inv(spaceToTangent);
    /*
    Vector3f n = spaceToTangent((Vector3f)isect.n);
    std::cout << "Normal Tangent: [" << n.x << ", " << n.y << ", " << n.z << "]" << std::endl;
    n = tangentToSpace(n);
    std::cout << "Normal World: [" << n.x << ", " << n.y << ", " << n.z << "]" << std::endl;
    */



    Vector3f wo = Normalize(spaceToTangent(v));
    
    double pSpecular;
    double pDiffuse;
    double pSpecTrans;
    CalculateLobePDF(parameters, pSpecular, pDiffuse, pSpecTrans);
    double p = random_double();
    double pLobe;
    BSDFSample sample;

    if(p <= pSpecular){
        sample = SampleSpecBRDF(parameters, wo);
        pLobe = pSpecular;
    }else if(p <= (pSpecular + pDiffuse)){
        sample = SampleDiffuse(parameters, wo);
        pLobe = pDiffuse;
        if(parameters.roughness <= 0.01) sample.isDirac = true;
    }else if(pSpecTrans > 0){
        sample = SampleSpecTrans(parameters, wo);
        pLobe = pSpecTrans;
        if(parameters.roughness <= 0.01) sample.isDirac = true;
    }else{
        std::cout << "now u really fucked up" << std::endl;
    }

    sample.reflectance = sample.reflectance / pLobe;
    sample.pdf *= pLobe;
    if(sample.wi.y < 0){
        sample.hitIn = true;
    }
    
    sample.wi = tangentToSpace(sample.wi);

    // std::cout << "Outgoing: [" << wo.x << ", " << wo.y << ", " << wo.z << "]" << std::endl;
    // std::cout << "Incoming: [" << sample.wi.x << ", " << sample.wi.y << ", " << sample.wi.z << "]" << std::endl;
    // std::cout << "Reflectance: " << sample.reflectance << std::endl;
    // std::cout << std::endl;

    return sample;
}

Color Disney::EvaluateDisney(const SurfaceInteraction &isect, const Vector3f &v, const Vector3f &l, double *pdf){
    PrincipledParameters parameters = isect.parameters;
    Transform spaceToTangent = ToSpace((Vector3f)isect.n);
    Transform tangentToSpace = Inv(spaceToTangent);
    // one day we will add normal maps

    Vector3f wo = Normalize(spaceToTangent(v));
    Vector3f wi = Normalize(spaceToTangent(l));
    Vector3f wh = Normalize(wo + wi);

    *pdf = 0;
    Color reflectance{0, 0, 0};

    double pSpecular;
    double pDiffuse;
    double pSpecTrans;
    CalculateLobePDF(parameters, pSpecular, pDiffuse, pSpecTrans);

    double dotNL = Dot(wi, Normal3f(0, 1, 0));
    double dotNV = Dot(wo, Normal3f(0, 1, 0));

    bool upperHemisphere = dotNL > 0. && dotNV > 0.;

    if(upperHemisphere){
        if(pDiffuse > 0.){
            reflectance += pDiffuse * EvaluateDiffuse(parameters, wi, wo, wh, Normal3f(0, 1, 0));
            *pdf += pDiffuse * dotNL / Pi;
        }
        if(pSpecular > 0.){
            double specPdf;
            reflectance += pSpecular * EvaluateSpecBRDF(parameters, wi, wo, wh, Normal3f(0, 1, 0), &specPdf);
            *pdf += pSpecular * specPdf;
        }
    }

    if(pSpecTrans > 0.){
        double transPdf;
        reflectance += pSpecTrans * EvaluateSpecTransmission(parameters, wi, wo, wh, Normal3f(0, 1, 0), &transPdf);
        *pdf += pSpecTrans * transPdf;
    }

    reflectance *= std::abs(dotNL);

    return reflectance;

}












