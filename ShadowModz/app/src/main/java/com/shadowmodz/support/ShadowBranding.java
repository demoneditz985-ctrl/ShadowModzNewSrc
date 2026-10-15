package com.shadowmodz.support;

import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.util.Base64;

/**
 * SHADOW MODZ - branding assets.
 *
 * The round logo is embedded as a base64 PNG so the login screen never depends
 * on a file that could be missing at runtime. The same artwork lives in
 * res/mipmap-* (app icon) and in the native menu texture
 * (jni/SHADOWMODZ MODS/ShadowLogoData.h).
 *
 * Telegram : https://t.me/+BBimnHMiSvpiYTBl
 */
public final class ShadowBranding {

    public static final String BRAND  = "SHADOW MODZ";
    public static final String VERSION = "v5.2 PREMIUM EDITION";
    public static final String TELEGRAM_URL  = "https://t.me/+BBimnHMiSvpiYTBl";
    public static final String TELEGRAM_TEXT = "t.me/+BBimnHMiSvpiYTBl";

    private static Bitmap cachedLogo;

    private ShadowBranding() { }

    /** Round logo (transparent corners, purple rim) decoded from the embedded PNG. */
    public static Bitmap logo() {
        if (cachedLogo == null) {
            try {
                byte[] png = Base64.decode(LOGO_PNG_B64, Base64.DEFAULT);
                cachedLogo = BitmapFactory.decodeByteArray(png, 0, png.length);
            } catch (Throwable t) {
                cachedLogo = null;
            }
        }
        return cachedLogo;
    }

    private static final String LOGO_PNG_B64 =
            "iVBORw0KGgoAAAANSUhEUgAAAGAAAABgCAYAAADimHc4AABMSUlEQVR42p39d4zl2XXnCX7uvT//fHiTEemzqjLLZZFVRVJ0" +
            "oiiRaqll2T2LQffMADODwe4A+0djgcEOBMwCA8wCPcDOHz0GY9Tq1XRL2jZsUsOmKFJ0JdEWy7Aqy6R34d178ezPXLN//F5E" +
            "RkZlVVETiYvfi98z+eJ8zz3n3GPFn/69keBv9yMecT1Y8shVja/e+LH3iOWPl3fkWi6HB0IJpBQICUiHkw6rrDDSYSQ4AQ6Q" +
            "TiCtdMoKlBUIC8I4rHVYC84gMIAer+JDrhoOX2/Gyx5b7sjiEdcPo6Fj/Me+5+b/CeIfJ/zR5R0j8sEKDh47nC+c8EH4AqXA" +
            "+lqkYSa6lUx2G4UYtCx501A0BKIqkKERRewwPjghnCokfgqkDj2ULtiX+B3PJe3Q1fdDV+/7LkklqnBY7bDaCVcIRDEmegHk" +
            "Rx4fLHUMEHPkbz4AwR6hj/sF6Xj4vPd/kviP4vrjRD/K5cGRawCEDhcIJwKB9AXC12IUD+VOayR3ThRidBLEsueCWc/Fjcg2" +
            "Y+XCQBEgnYdAgXAPf2UncVisKLBoNKkuxGjUlxvdjriz7Zy977nwTuwmVypuZidw1SFQOEzhhMsFIh+DkB0BJD+2O47uCnGE" +
            "+O4RIPwiu0GIX0AEiWOPjxNfHbkeFy2HBD9YDhcKJwOBDLRIk75anx2KrfNOmIueS05HtjERuHrgEYIAKzOMN8SoIUaNsDLF" +
            "CoOVGoQtcUAirEI4D2kDpInwTIIyFZQJwSm0y8lFT6ei3clF/x6Od2I3ebXuTqyFrtq32BzhcofLBCIbA5EfuRaPEFHmfcTS" +
            "LyySPgyADxI58n1EzQG3hw+Wi4AIJ0OBCEdyt7Wv7l3QYvSRwNUeT+zURGDrUkiH9rvkwS550MH5IwLfEXmKJAyIwpA4ivA9" +
            "H89TKE/hHFhj0VpTFJrRKGWUpwyzlKww5Frgigi/qBPmU/i6iTCKjL4byp1uKjo3hPNebdilt2pufkcgUids5nDpGIj0CAjH" +
            "d8VxINyR6y8EwgcB8CjiHxU38pioCY6saEz82OEi4WQkEFFPrs/01L3LID6W2JlTiZ0KhIQ83CWN1nHRPlEEE5Ua0xMTTE43" +
            "acxUSSYiooaPSiQiFEhPgAIx/mbOgjMOq8GmhmJgyPY1g52U/a0ee1v77LTbdIZdhrmFvE6UzhLls2A8RmLP9MXGqiZ7uWYX" +
            "Xm66U6sSORoDMRKI9AgQR8E4KprMsZ1gfxEQ3g+A9yO+fB9xc1TMREDkcLFwIhaoaCA3Ztre7ReU8z5ZM4tLEU2p/T6j+D4m" +
            "3qaaKBamJllcmmPm9AS1xQi/pRDR2P5B4HA45zDG4qwF57DOIYVECBBKIqVECPGQNHbGYQaW4XbO/r0BO7fbrK1us9Xdo5dp" +
            "ZNaiMlwmKCbJXd/ti/vbuej/qGGXf9B0J9eAkRN2JBCjMQijY0B80G54P0vpAwH4IOIfV7JHZXw05vhYOJEIVJSLbnPXe/c5" +
            "h/t8zS6diV1T5uEew+otVNJnodnizOklFp+YoXYqRDUkQomS0Noy6KZ0O13aux267R79Xp/RICPPMowxaG3wPA/PUwRRQJzE" +
            "VOsJjVaN1kSDarNKUo3xfB/EGMDMMNzM2b3WY/3dHdbWt9kediiyiHhwiiRbIHcD1xZ31w3Z96bchR9W3MyOw6ROuOEYiAMw" +
            "0keIpb8VCMcB+EWJ7x8jfAQkJfFlAiR76tq5kdr59apZuFy1c0ERdhhUrxNUB5yenefCk6eYebJFOOeBB8ZYsn7BsJ0x2Bly" +
            "4+oKt+5ep7O/R5amGG2w1mJdyf0Od2hvCAROgJQSKRS+7xFFIZV6hcnZCZZPLnPusdPUphL8UCFkCXLaLdi93mP99V3W7myz" +
            "MdhhlPpU++dIsnkG7JqOuHPVd8nXZ91TVxR+3wozFIjhGITRMR1xdDfYMRAfCMKjAPgg4vuPIH4y5vyKdCrJRLe14731Kc8l" +
            "X2yaM1PC0/TqVxHVNudmF7h4+SzTzzTwJyUISzrI6G6n9LcyutsjOhsDbt27xcr2DVIzxFiNcyXhSwBKTnbOHbHlxOFSQpVA" +
            "HFmRqvHspY9w+tIiSTOmNVOl0gzxQw9wZMOCresdVn+6zfrtXTZGO+TDCo3BRbyiRps7/YHY+e60e+xbNTe/5YQZOtzgCBBH" +
            "d8NRa+koCI/UCd4HmJvvR/wDWR8fEF44UZF4yb66s9yV93+7bpdfqLgZb1i9Q1a/xYnJFs9c/hgnXpjCmy65b9TP2b3b45Uf" +
            "vcnqvRUqXhMvDNjYWmNj/y6FTjG2QBuDsRprDc45rLU4R7kDjgJAeWaWhwAoPKVQymOkUq7dvk69WSfva3bu76MFTJyos3S6" +
            "SVINWXpmmqmzdVpv1mn8qMb2+i4b4U/wBotMDJ6oJm7yN3fEtTMDdr485568irDK4dTBKf0Y8x786CPEl+OreNQO+DAz8yjx" +
            "ozHxkwPig6hse1ee0mL09yf0+dPSE+w33iSpj7j82ONc+MwyySkPJIz6Gfev77BydR+VKe7euMc7Gz8jNylaa3KdUpgcY3S5" +
            "rMYY8zAAvA8AQiClQgiJkhKlFJ7yUSog8GKmG4tcOPU4Z86cwI9Drl3v4VclF5+b5szjU8RJgMXS3tzn1t+ssfHzfdYH2/SG" +
            "glb3GTxdY5t3ty35lxfd8z+Swus57EAgBsDgmEgqjumFR54V1Jcu/YF8H5/OcWV7nPhV4WTVCdvY8F/5pET9w0lzcUHHbXqT" +
            "r3Jytspnfvl5zn5hjmBWYrTl1tsrvPK9a6xd3yUbWDIdELs6O3trbPdWGKZd0mzIKB2RZiNGeUqap2R5SpZnZEVGnufkRUZe" +
            "5GR5RqEL8qK8V+iCXBdHrjmFLtBFQaFTusMdVjfvs7PVpVGZpBU12dnUXL/RZmu1gx8U1JsJ1UbM9NkWqgluT+Dnlh3/Js4o" +
            "JvXjlYL04g7vigpTa56IrMMK8YD5j4oY9yEHMaG+dOkP3u90+yjOT44S34isue69/GuRnfh7LXe2Majdopi4yuUz5/nEbz1F" +
            "6yMVCKC92eX6a3d58+Xr3L+9TRj61BoR797awRUNzs8ssL67wXZ3lVE2OCR4nmdjYperKHIKnaN1US5TYLR+8PuR+4fr2P3c" +
            "pLQHO6yvbzPXnGZpfpb9nmV9bYc7V9+h3+3RmmpQqSVMLNSpLkUMuxlxL6Tr3WdAj4niMV867/wWb8chjZVAVLTDIt7jH/lw" +
            "MI7ugOM+nUcRv3JAfC3S5ob38m9W7dzv1N1SvN+4gj+5wSefeZanf+cM4bJC54YbP7/HGz+6Rm9nQKe9T3u/jzGOLDds7YzQ" +
            "WZ2PP71MXI14/for9Eb7JcF1dsjBhc5LYlpdLlOMRdNYRI11hLEGY8xD94014/eUq3y+oJ+12d7d4/TcSaqVKht7u/S6a2yt" +
            "bLF5b4e4GtGcrFFvJUydadLL+vi7AbnaZY8NWvqc8l3lzCZXaiH1O6GoFmMQOEZs+0EHsQMAjsr9owesR4odI7Ix8U/8Zo2F" +
            "oN36OdWJPp/9+POc/zsLeC3JYH/IT771Oq995x2cdjghSFPN5vYOeaHxRcwTpx5jOqixvn2bb/7066zs3aAoMgpTHC6tDwhq" +
            "xrrAPljuwWMz/t1YW4LhzOHzxozvjT/Hjq/dvEt/v+DM0ln6dki7cx9nNYNuyurNDbCSqbkWlXrM7MkWfQaYDYkQI3bEPRrF" +
            "WRG5xslNrtQiGrfHO8GNd8KHuazdAQDqGOcft/PjB5wvqk7Yxrr38q9V7dzv1FgI2s1XaE0U/PJnX+TE5yeQiWBno823vvoS" +
            "b/3sOkVfo3PLYJjSH/VRMsKzLR6bv8jymYTv//Db/PlP/wX32m+T6xRjCswBt9oxEd2YiM6W946CMF4HCtoeeU8ZDShPztaZ" +
            "ctnyWn5OQae/x4S/wGzlBKs7qxS6D85gC83W/T30yDE1P0Fcj5ldnmTkDcnXHb6wbIo71IszInSNpU3eDitM3fJEqB3OHRFH" +
            "7n3OAhwF4LiL4T2iRzhRA1Fd9372ydhN/L26W4rbzZ/Tmsj53C8/z/znmshAsH53i2/+m+9x453bmMyQZRn5sGDYG7FwZoq5" +
            "6SWm60tMNkP+f1//N3zv7X9Lr9gouXIsPrQx6LFIsc5hrD0CxoMD2eHiAYEd7vA9pfvCgntwfrDOYpzFWYezjsJmjPqai43n" +
            "GA4s3WINbXKEE0ReQrqT097aZ3JxgkojZGZxgqEcMlrTBFi2uEuzOCd8F5/c4G3bYOGOFMqOQbDvcyJ+aAd4HyJ6qg5XlXjV" +
            "Le+NZyTeP2zZs4395hUqEz0++5kXmP9cCxk4Vm/t8M2v/pA7N++S5SnWOHRmGAz7+DJk4eQyp0/NM7/Y5N/+u6/y/Tf+nEx3" +
            "wEEYheRFjtb6CJcbrC2/q7Xu8EDmjv9zJTGB8ozgHryHg4PbGAx3CIRDSoHD0kt7LDdOc7J6mrt7t8hMD4VgobqAyQrSbk63" +
            "PaQ5U6PWjJmebzFwA9I1jaRg260yWTwunbOndrix32R5VYiHPKPv66RTX7r0B/4RAPwj3swD0VORTlU78s6pTHT+wynzxMKg" +
            "egtvYpPPvPhRln5tEhk6Nu52+OZXfsLK3Q1G2YC0sBR5jjPQG/VJogaTE7Oce3yBv/6bn/Jvv/tnZHp3LL8NxlnyIkNbXRL5" +
            "yCrPAGPiHWOhA84WMCZu+XcePH4AUrlTXHmKK/1CY9GVuxSrPV5cfpHNzh57+Rq+k8zYKWzhkIHPxn6Hna095k5MUWnGTM83" +
            "2O7vYjYFiCEdu8uUvuiP6Jzss3W/LhZ2x/rAvA8Q7uAc4B/j/qO+nYpwspqL3uSud+3fmzJPPF1EHfKJa3zi0tOc/80FVEWw" +
            "u7bHX331Ze7f2yXLUwbDAYWJKLIcqWAwHFBzUzy2fAGL5o/+5R+xsX+jlMVjcZMXxQNCA0rKsVw3Dwg45uRyN7jxIYxSnBwh" +
            "7IGf6OA1TjzYKYevGYNZgm1I85Qn558nsBVWunfwrGVZLAGCXddlKDT9/Zx+Z8jCyRmSeszUQov722uE7Yih2iHVhkn9WNLm" +
            "zqxAXo1Fc+SwdgzCo2IG7lGy/xAE4UQCJJvqyqcaZvkF6cGg8RZPnjjDhS+cwKtDr9Pju1/7Eau3N8nzjCzLSXONtglZDp32" +
            "LmmWERUtWsUEL//1m2zvb1CNa1gE5lCxjk1JYwgCn3qzgROMrZnSqjFjS6Z8rcbzJI1GHYQrTU6nD3WBcRaLKz/fPKywjdVo" +
            "ZygOlL02tNMtrq69y3JlmYqYwuYQm5iRHrEx2GWU5WRas3atw5vfvkY6yKk1K7zwq0+Sz46YD2dJqzdI/V1mePzCtrv+xcKl" +
            "NYGIx9IkOhKsOkhUkI/ych6EDmOBinbV1XM+yRerzHr79TdZnGjw9K+eI1xQ5FnBj779CtffvkmuLVkBgzRlmA0Z5SnWSLI0" +
            "wxqJooIZwLVb9/j4Rz/D/OxJtNHYQ+vlwJopr2lauiYOLJkDHeCOOOO01gyHwzJG4MqdYY59zoHiPficA3H38LnBkOuUe+0b" +
            "NCoxk/4MkfbJi5yVdId+PmQ4HCELSZWE269s8u4P72ELy+yJSZ755QsMkgGL8TS7yev4MqIu5j+15l6/LFCxw8VHglRHAVDH" +
            "d8Ch6SmciDPRbQ7Fzq9PmDNT/eQOSW3Ecy9cpP5EDBau/OwaP//Ju6RZwXCUURSQF448z8myfukqKDTCSppRlaxrma2f5dkz" +
            "z7O+voXWOcYUJdfiSi53huFoyP7+/lgkgXPiETEhidaW4TDFOR7yhzknDsXUQyAcsZCMtbgxCNppjM3Z7K8jPMNkMEnT1djI" +
            "dtgocqxrIEyVCTWJ6wlcHvDOD+9z7+1NHILzT51k6dlJnOeYSBS70RtMiJOxRf+djrs/J0sQDgB4aBfIR0W2HC4SqGhXvvtc" +
            "zS5exjOktVs8eeo8i780gfIEq3c3+eF3X2E4GJBmKaM8ozfskmUj8jHh82LIoBggnMRzPlEe8psf/QTtjV32utuHJqVzHLPl" +
            "7eH9h3I5jrmhj+6G4/cf9fwBEJ5XxgsOdIy1FuMMo2IEwtLyG1T8CQxNlqsf4dm5T/KxpY+w5Kap2gRroN9L+fF3r9DZ7BAE" +
            "imc/+RhuQdMKmphkg5G3zZQ4d3bH3fi0dSYZi6L37AJ5xO0QHATQhZNRX6zPWGE+X3NzQbf6Lgv1Buc/eQK/KRkNRvzoO6+w" +
            "s7nLqBgxykekWUaqC9I8pdCGQjucLY8YTijaRYqnYHYy4Z3bN0nNcKwEKVOnrMDZA05+L/EO/P2PAuJRAD2K+A89P1bAlgeg" +
            "g8IzkkruozNYbp7jl86cZ6kuOD/RYjGZxHM+hS7oFR02Vjd57W+uUuSGiZkmT33iPP2gx0I0xV78JpGoEVH79La7dkqgwvEu" +
            "OIgiPnIHhEAkEFFb3Xq+YZfP5GEbUdnj0sWz1B+PcBbeee0mN96+S1EUpFlKmqUURU6eZWhj0dogiAhUSCOoUdiM7cE2zof9" +
            "fp9727fHtruHtWVmgxAChBgT6eFAy1GufnRyjUAp9cj7B++11iKlJI5jiqJgMBiUOXVCPgDAgdSKvCgICDk9PcPO4C7//Ht/" +
            "xHa+SzQTIKSkW+yzn3cY6Zyrb97h9tVVAM5fWqJ5roLwBEms2Q9uMyHOTHZZ/7R2WUUg3iOG5HtcD06GPbk2I53/qdi1ZK9y" +
            "lZMTM5z42BReKOns9Hnth28zSoekRVpyfj7CGInRkkJnGOPwVYzvVUrFmRsmvQaT0zFDP6U9bBOogI8tfY4npp9DyYBKNSYI" +
            "gvcQ71HcfRSYwwwzz0NKied5DxH+6OsOgDoQd8YYCl2MTVtohi1869EZ9RjpDKlzzizO84Vnf50TEzPooWOtv8lK9x5ZnmMK" +
            "w6A/4rUfvM1gf0hU8XnqY+fpJwOmg0l60XWU9Kgw8dFtd/W0RAVHdsF7AXC4UCDCfXH3ct0tLmXhLkE85MLFUyTLAcY6rr25" +
            "ws7GPrnOSjdDkZHlQzAetWAZrI9zoIsUnRdkaUHiNXjq5Am297t847Xvsj9cRTqfJ2cv8/lLX0DJgOFwQFHk78vhR4kuhMDz" +
            "vIfAybIMIQSVSgUp5SM/pygKut3uIQBH9YIk4umZy4hM0B7tkusexvSZ3a7xuZkXUJuKt++u8Xb3Fv1iRF6YUpkbzeadXW5f" +
            "WQcnWDozx+y5FoXU1CLJvn+Tplhu9tn6uHZ5IhBHd4D/kPgRTgZDsdNCiBcj25Lt5GVON2eYvdxCeZLdzS63r21RGHMYICny" +
            "nKLI6ZldkmACj7A06wpDYAf4BCR+yMKpGnfu7PC1732LbrqFA/6/r/4vKOmR6R7O6UdE9HhP7Pfgntb6PfettfR6vYd2ynvD" +
            "3e7I55bZhQ7HiepjfOL0x7i7usFWus4JEZGtpayvd9Cx5Z7s8OP+24y8UnQZ68i1RglJJa6x+vY2p59apFKPuHj5NN+59jKT" +
            "tsXt8A7N/ByRa1zec7e/Oy0uDBwmBBEA+cEOCBwukKigre5eqNjZ09rvIeMep84tUjnh4yzceGud9bU9Rumw9Nnnpb/eWkdh" +
            "CjqjDdKiX8pw4aOEz0wySZoNePOd+1w4P89/+Tv/iE+e+32awSkiv0mq93HOfCjxj4uU4+IliiKklIfcfVxEPdgV7og4Ku8p" +
            "W+ULz/1dFqpTvLZ+lZ7eJZAhkUgYmJS32nf5/s5r7OgdLD7OSqwVSOcx7U8SiYh0oOlslOCcODPLxFIVIwxJ5Oj5KzTFiel9" +
            "t/bsESXsA748eCCcCAoxSjSjjyRuKhjEd5mu1Jh9qoUXKLrtAVevrJOlGZnOxmHAHG01URRgTIZwMcL6eM6n4k2S+FU8C5iC" +
            "d1bf5LWf3eDx6WX+q9/4R/zj3/9v+X//Z/8Ny5OPjy2QR/88Sq4/SiFrrT9EUUvEEdHk+z61Wg2EpBbM8vTsU9y60eHe4Dra" +
            "ZYzyFCEdvXDEz7ObtOmCk2UwqdDIQrEgp7FaYKSkOVFj526HbJATJD7nnzrJwO8x6TfoBrcJRR2Jutx3202QhyA8AADp98Ta" +
            "TOhqj0kp0PEOJ+bmqJ2KAcH9m9t02ylKeWMRM469Fpp+f4Q1lB/lPJTzqHoNmskEIYLICYbpDi/f+AHf/ZtX6Fzr8dTMMqfr" +
            "C5ArhLDvy/1KKaampg5BeJS145yjKIr3AdIdBvyEkLjxTlNKEYYhCEejMkut2eD2cJP9YhchPEIR0zcZd70tsjBFIsFJsqyg" +
            "5TU5W1mgM8wYWY+kUiGpJmze7LB7r4vAsXxunqDl4SkPGfTJZZeqmFluu7unJcp3uAdKuPxF+AOxeaHipiezYJtKALPnJglq" +
            "kmyUc/faFvkwxVmBsSXHGW0xhaHINFqDI8YjJnYxWlsiP6Tl1UlkRKtWZaQ73Fq5yv2tdTJTsLveo5vtHxLlqNw+uBpj2Nvb" +
            "wxhzuCOOWzaPeuwOd8uDHYB7IOjSNGN7ewdTCE42TpMOfW7ubaBdQSwSEq/OnWyHd3c3QMY452GsYqE2zZnGAp2uwZoKFZXQ" +
            "aNRwzjFop6xe28FqqLcqzJ+ZZiCGNIKEnnefqpiOUnqXrNOBQDwQQcKV+flW6Iuhq8tRuM5UpUXrXIIUks7OkO3VIXmm6XQ6" +
            "jNLB2E9T+liElcRhyPTENJNenaas0fLqxEnMdHOKxPjMeVPMeC0qQtEA4p5kNMiJo2kunn0eRVQWvByT99Za8jw/lO1FUXyg" +
            "qDnyAYhxhFYIkEpx4MwWlB5Uoy2+i3n2iWdxswPawzU8BzFVUmcY2IJBkWNQSBdyceYUF+rzjPYifDNBVUZE0iepBhhTOv22" +
            "VvYZdFKUJzl5dp40SKl5VUb+Op4I8Qgf67PT4AgAnkD6A7Hd8l3ltJBgw32mpyZI5iIcsHG/x2hf4xMgncIUGlMUWGPAOZRV" +
            "TERVlisRl71zvCgv8YSuUlmxtLwWS9VFJlyFc/4JpoMmvvRQucIPfR6bfZ7/4Hf/rzSqS9hxrOKhw5c4nph9jNPfFwzBAfWF" +
            "kCUU7qCQQ4ztIUsjmeO5S4+xu7VHP98aZxw59k2X3GkK68D4PHfqHCfjafxOnSlvikhFFAZyW2CxxLWQMAkYdnP21rqAY25p" +
            "krgRgBR4QUYu+yS05rtubV6iPMDzxgCogdhZjF1zIve7BL5jYqlOUPHQuWVvbURNh0xrgcja3DcFduzwwkBsAqKVEZ/55RYX" +
            "vnQW0yko9g3bPwkxrw9pNio4AZNO4k2G1BoJgzRlrjbBF05/ntMnYiabs+z0rn9AoYIoT8yylN9HzVApJb7vk+f5Q6/HlVnV" +
            "vl9BCoVwouR/VwZyGkmLzz7/GeYWGuz2M3wZYExZfFNXVbSNqUYtPnrhBLOmiupUKaRjX2c4k5MWGoYp63c6VKfnqLRi+isp" +
            "W/faLF2aptKImZpvsrPVpeKHDNQmiZ5Ntty7p8G95XAlAA7razE8FdqTQRqs0AhiaksxUkoGvSF5O6cmQqKiQGUpOs/wnc90" +
            "PMlusUtkAk7aGeamGtQ+GuGIEUJQfyGh+093sD8boocCEwoCLyCoJOjMMhU3eeKc5X/71/+aveEuUVQnz7ol147Diw9C2A9C" +
            "jnqsDx7Y+3LMtwfElzgnEdJHqgZ+NIVkQKFzMIbAC/EDn09e/lUCH3r7+yRUqPgtekUbTwRI4TFRq/ErZ+rM5BW8QYILJPuM" +
            "yEzBZr6N83IkEWlmKFKHycA5xebqPmm/oNIImTsxycqbW1RUlU1vi2Z+Guvc6cKloRK+5+HwCjEKcWLZI6Tw2zSSGpXZCBAM" +
            "d1O8oaUW+Piez4LfpC5itvQuaZ7xZOtxdJRS3UoIwwg/9kuXsHKoxxLi/+cio6/0MD8pyFODmpR4sxWKVYdfEWy09/jxWz+k" +
            "n22VClYpJB5GF4hx+rNzFidK9j+Mlo1TFQHCoMIo65eKFodAUqnNMr9wipPLF/HlBIPBCq/+/LukWY8nzzxDrVnh/u4K2zLk" +
            "nftXmU5fJBTTDN0ae2LAWnGFL7QmWRwsEpsKWjlSrenpETcHG2y6XWKlCPMIJwLCSJHUI6JuxmB/SK89pNKImZ5r4QJHoH2M" +
            "2kMIgS/ChSHtao3ZrgShMroVRTgrpMN5Q2qNCmHDx+Ho7WZ4ykfWQoLFOslkA+mFOBRFYXBtxZnaWc49fgavIvEqClWReJFE" +
            "+Qo5HRD9+016H1cUFYU6G6AuCEQLTMvw6o2r9IpNpBRoXSCFRxBUCIIKnh8jZYCU/qHpqJSPEBIpPfzx86PRYBy8lwgRMNla" +
            "5rmnvsCnX/g9fuOXv8SFE88QiUVqyRKt2jyfePLjePisrN3izNJZ7q3uMDGtmGtO4JTjvllnGDkmBhNM2iaeVAgh2Mz6vNa+" +
            "y0q+ThnB8HD4ZKmls1YwuRQzc7KKzXPa2/uAozFVI6gGOOFQnqYQIwKqE0O315RI6QmkTMV+wyduGJmhPEt1IsGPJKYw7O0N" +
            "GdoC54FXE+hC4u5ZhAyYTs5y3n+WfEeDX4NCYlKNNaAzzaA9xIt8HJKt6pAogWjoowJDcFoxiHLeXbuGExmm0HheiHEeQnrE" +
            "cTIWMwowGJvhnEVKhbXl6dv3wrFJOaRZmaLd2yZOqlgc12+9yu52wTtvDsiLEZ3+OxS6z4XZU5yfXuTbr3yb/mjA91/5Dpcm" +
            "P8dnTgh8EeETUnUBNelIYh/lKVJdcK27x+vtG6zoTYS0+CQoEWItaAP7OymNRQ+dG7SGzvYAZx1JNaLaiMm3NaFS5LJLaKpJ" +
            "321NCaTyBEIWYtCKXCvWakigFEkrRPmSfGjo76ekOi/99UYxDEMMAQFVHpu7zGPJRegU3FndpL2aEd8bEM3GqNijVWsiPYFU" +
            "gsufDOjfTTHbluy1nOSxEAqNyVKEOIhcaeKkgjUltzvnaNRbhEFAoVN6/R5aa4SIyjIlQEiJ70cgPcIoQYqg1BO6T7v7Bv3h" +
            "NbTJyXUfgE9cegHlCdqDDtrktHt9GnNTdHZ9Or2cwPpMu1mSfogawY7q89LWfe6lHYa2T6AifD+hmiQsz1WYaAZ4nqA5G2KG" +
            "ju5aik/IsJ2jtcUPPOqNCmvsEqqATHapsOBpsimHkxKE1GRNz0WBkQNCqQgb5TbX2lFJAmqthKTmI0NFXK9Rq9YRCHZGI/Zd" +
            "TjQdYn2P+lyV5tkaUdMnSDykV54+hVCl1eQLhFLYrqS4A/U44eKZi/iihu+X7ozAC/FViMBDSZ88TcnzHD8IxqnnpQkqhUJJ" +
            "D0lpYjosUVxFeerw/FDoAWkx9mxaw9LEAk+efIzd0T7dYRcVxEwmy3z07DNs9PbYzTcohEY7Qexq7PdTfnDvBlcHW2TCIFSI" +
            "kglCBEjlY3AgBY0opBnF9O8N0aOcVKd0exlFXiCVpF5PMEITSp9C9vBEiMVMWIzyHFY6TEMRUKiUwPMJqh4CgSkMyhM0J0P0" +
            "wCfwA3S3YD6aYsfb5nr7TdL9lAU7ybbOWPvWFjO3T/PYx2Ke+dTcscplgTMgpAMNeq/A35P8xuc/yVv33+X1+9/DeprhsE8Y" +
            "1JHCQ0gwzjJKRwxTh3MSpYKD7KsHlpG1hwcsIcU49USM68LAVz4SwxOLZ5iuNnjpnftkec505TH+3sf+IRefO8sf/osfsa/v" +
            "4pOjZIIEejplrxiAr5Ceh7TlXyJlhBIBwy70C83UTMFwNKAICqKpBHJFIBSmsAgBlVqEEQZPeBg5QqIA6sblvuewCkRVorAy" +
            "I1ABMi6dVsVQ09vOaM4m1OYiksSn2A2Ya8yyGq+zo7dY6e8wMDNsq5CVV/t4P1vjP44/wuVPL2KcGacHu8MAiLWOwjmKvkZe" +
            "9zn93AT/99/6D/jDv2zy6q3vMtDbxEEFJX0sBglUggojU2ZM+55X+v/zdEz4sSg6fiCTEikkofKIVEy1VuP0/DLW09zb3uPS" +
            "whf5nad+m898/DJX2/e40fkpDeOISfCEj0GDcEjlgxSgJM5A6AvmmiM8f5XU7NO1mmuDBN15nLm5eephgBAx2hQ4XTJJmEQ4" +
            "aVBC4kQ+NpRV1VAoz6KlQIQShZMaT/l4QenwMtqgjaW/n5GtF9gkpnWqyqknlri/uUY/62FGA5oVj56TeKJAjTao1N4bsQKH" +
            "qxtG3Rw59NAZ2K0M84riiecX+C//i3/A9/7iI7zy9js0qhEr+9e5s/kOSjnOLl5gfXeNvf4uURBhMBRFhhWlj1+IsjrmKARC" +
            "gic95lsncAaWZxdZnltA+/DsxU/yyf/oBbbf8Xjp55tk0Rq97CZVt4h1EUiB86DnFWjnEXsRnnRUJiRrnR+TFju8eOHjvLay" +
            "z+srV6hHCbudXWaaJ5hoTTM9M0lrqn7oHAzCEkQhBE6Y8vviB4bc85yw0pLH4LCiwFMhyi8ByPIC6xzpoADrM0g13m7G0qU5" +
            "zq6dojNo0xlmeAiEzUnzPo1ghmozwR3xcErKeK+IHPEJGL6mGfYhmpAM+n1wEdMnq/zOF5/jC3/3WbRf8PW/+CkbX9/E91JC" +
            "Qk5NnKYR1dgdbLM/GqCUhxD6MBdUSnnogDtsfuEce/09POkzGO4zNdEinqjxD/5vn6NWq/L/+sZf0O0bTiwpwnqC6QtyU5D5" +
            "OVtpj6Y9x2KjRWs+IW5a7nTe5scrr7A3hL3MYVTAMB/hnEXILYSMqMVTpG3L6mCTM/15mrO1sjJTiXE1p8FhcNhQu9zzHFZY" +
            "jO9wlF1eHlSgF6nG5hoZ+GRFAVLiDz2yTsbjz53h3p37ZPtdsAJnDXgBOk7A898jEhwOuQVe4FE0gDRHnQQ16djY2CO7L+hd" +
            "TZl83GfqTMxHXzzPT394krmpCOlJrq9cZ5B16WcDrC25KAwDsixDHoQry+j+Q//rsBgRyIL56Vlq9RpeHBJVA/a7Q/xKh/nJ" +
            "BlFVMbewRH4npRhqEpswF5/gzFILfzkkTQ3Zjs/+ZkpRpHSV49reLYRQBL7CuILMZgx1RuRVacUTrGX3yIviob4d7iCVsmQV" +
            "3wkjvUdUSB7EjPCUwmqLDKAyUaEYQd61DLf2mTrbYnp6iq3VTezAUOiUIu+hvJBRWx+r03FlLWEsEZkjXHTQs4RLMfVnIno/" +
            "2WZvr8dmL0MOI5KhT30iohFX6aVd7mzfpJd2yE2OtsW47sSS5yCFAClxVhyehN1RR50oo3ULC4v0dI7eguh6ha7r8ltfep79" +
            "TsbtW7e4d/06cZEwISawzpGbAp1q4u0mTRGgQ4MWO1iRIp1EG40nfazxcUpijCGzQ263bzB03bKxCOY9TMhh2YzBgfAEwglU" +
            "wdh5Na4BPXCi45wl7RU0WprWiSrpuiXVPgYL0gNfgHUoJ2lKh26/yXDjNM6eRMqxH77stoTLoXDAREi0UCAa4E0rzv7yNPud" +
            "gv31nNZcTNLyMST0zR5X776CU5Q5nNjxd3uQKxQEwTgoXxx+Z3E0OctakqTCybMn+P7PfszjZ5/l4nOnqNZ8Mptx5kJMb7CD" +
            "sKUd5YSgEJqRzamkVSaSKrmxpBTs6zaVJCFP8wfZ2wc1m86g9Yj9dB3XHVGvV3FjA+EwJjF2KI75XUsnrRROWYWfCgTC+WhT" +
            "+snBoZTE5ILA+PR2NP3uiMqST2XWozWfcOLUFElUQ3kBWMmzZ87x97/4IqfO1EppgCQflukbzgrMvsNmjtE7Gj+VmIFDFxI/" +
            "8UE6VMPhRxKdGaQv6aYDtvZXyM0I44px46uy4EKI0jg58IRKSWm1CIfAlf0jxgw0PzVDf9jha9/5BkIIVn7W5av/4xU6t1Ps" +
            "HcNkp8avP/5pPnX2eZRU9EzKnhmisejcoPycqJGT2R46zxFuXAt2IPqEQAiwzqBtXp7ahcULykJwnRucGccinEIiUfiZEoH2" +
            "JMqWnaYs0nllMVtWAI449mglPkHq0JHj5usrnH3mBLNnGniJYv7sLLWXW1zv77OW7fPM2cf4D/+LX8VLZPmlJOiRfsAlwwJX" +
            "lww3C2gafARbN4ZUfIeXWYb7hkJbhBR4QZk+OBh1UZ6kkrQOY7llOqFBiDLV5MAl/SCLbsxvQoJzNOpVvvyNf8Od9ZvcXLnL" +
            "RXOR4N2IXmNI7HvIuwGfP/UZ0izjrZVb9NClsvQcvlAQ5gziAf2sQ57lpU9KylL8Hbi+xyCIca2k53mHeU55VpQdXVwJQNl8" +
            "xOaKQHsCZS1maNEIE1CYHmZU5ugHFZ84lhTZCKVCur0uo24KfgMv8nAW6lGNrO6RZh3efGvId/6bn9FKfISnaJ5rMPPRKYLY" +
            "I7emVPBKoKoeg0qOqDnsqEdsI+qNiMVTVeKqj1QS6RzKA+cM3f4enhdSrTRotpr0+z1Go7SMxo3TSpSnEM5h9bgYfSz6lPL4" +
            "+fWfs7J6m7hW4d/+xb/BfzLiVy69QO4bbg0y7veG9IerbPTW2XMjiqJHZB3v5Ivc3r3Ojbu3GKgR+1nvgbI/DBOJB0tIlChD" +
            "pkHoEUQlAOkwRVpZBpycj3MWQz5U+NoTCCvx9w0FysbkZo+8XwY7vMjDScgzg1cYhtmQNM0Y7mdElQCHZfHkLGd0j366Tp5n" +
            "vHVrh8lZEOuw9vWf8tjpk1x4ZpmZS7N4l0L8BYWdsmQ9j73djEoUUF+MCBsRS4mi0ggQwvLum6vcXr2BQ+OsoN3ZwPN89vcV" +
            "xphDhXuQti5sqZCl8A9lrpSS/rBNu70BQvBbX/xN3nz7Tf677/zX3Nz8h/x7n/594sU6XivjzZ+ssetpnvrCp7BO8+YrP+Sf" +
            "Xv8jQi/BC6dYnJ/D8yI8KRFOHCmqPgx8lv/GYimOY4LQxwKDXop0Cm0NykbjyJ/Y9wi0B8Iogo4WmVYm8VKjSfdLb6PyJKqq" +
            "GFhNYhSpHjLqD8l7BdkwpzaV8ORnTyK5TyyWiWoRS3MtNtu7JLUqRbHB4ydPkegKvVsp1kBiAwrjiGoelakaMgc79KAOYaTY" +
            "Xhvwza/9jH/95/+Su2tvUa20GA77GKPZ2VtlamKRwI+OpKIXZPkQzyjCMEYqUVbMCMFo1KPT2cS6gsnGPC8++Slu377JoNjk" +
            "y1f+N673rvF/+d0v8eInnuHc2Sbv3G/zwytXefOtV+jub5PEp1mqv8DF6WWe+fg8L33/L7g+egntetR8ia98kB563JPCUx6e" +
            "UgRSMVGvEAYBVlu63SHKeWS2wLMTGHIEsi2FZzyw1ndJu2A4Cs1kbWAMo3ZWmp++xGuFFJ5HP01JzYjbd9aIo4TmiQrZqCxB" +
            "evYLp5i81WBvY4DnRXSGBZu7OVvpkP10iFcJGWSWtJMxW29gsLSWKkjlUznpkzvN9Wub/ORHb/P9732HV6+/RD/fJInrNOsz" +
            "CHx6/Ta6yNjdW2Vq8gS+FwMW3/MIvHrp/7EOlEMqwXA0YK+zhrUaJRWBHzPo90mSaunncj1eu/0Nrv2TN/j0977Av//bv8uk" +
            "ShD9BtONU4RIpsXj/O6lzyHSO3Rv3ePvnP88L48qDOy7/OpH5wiCgnudAd+4dgekQkpFqDwen57i8TMnUL4kG2l6nQEBAfu2" +
            "T+jq5G5oPIJdgTCexdqI+n5PbHSVXawZLRjsjShSQ1IPSeWAe93r5O0cL5SkdsjNG/epTSYsnJ8iHaQM0pyTT8+wcF6Tdg1e" +
            "VSJcm/rEUxRTAfteRjvNiM74VJ9JCOsefqQYdSwv/eBd/uql7/Hq2z9hffcWqd7BUoADJcv6wUqlTpan5MWAvEjZ3VtjanIJ" +
            "TwXgysQAMc6wdsAoHbDbXsXYAgcklQbSC9hub9Ko18qgr3BIoRnqu3zjtX/G27ff5D/73f+c33rqIn/8rS6d/h4feWKe6bjL" +
            "/+db/wQn4L/9j/8xvyG/wObmU+Srt3i1+woreo/cOEIYu0ME+1mGXy9DuqN+Sn9/SEKVTGuqtk7f7Y5CqjtlfjjOhK7Rb4s7" +
            "W8KqRUxErzsg7eYk9YiJmQbCs+hcY1xZzdLPBFd+eh1QnHpyGoSg30mZWKgydcqnuRQw2VCsvtNg5vEGU0sh9c0hOEcyF+J7" +
            "iv3ekH/6h1/jK9/+12z3b6DtYNyG1R1YCeXVOqRQRHGVQqeAIy8G7O7dZ2riBJ4KHjI7+2mX3fYGxpRHG88LSJIJrLOsba5R" +
            "q1UR4kGNQHk0Lbiz92P+5Gs1/ut/9F/x8Uvn+M7PdsikZfbxBmd+fJGlpQVmlyb54Z03+Nad7/LJx55ne6/JWraO8hW+8vCV" +
            "QkjJvnB4E1UA9nd75IOcmhMYLfFtQsbdTlOc2HNY4yEwvktSJ+yKJr/sFU26wy6DjRETJ2pMzjQJQp/dvT36oz6jdEggQ3ph" +
            "G++KR9HPmD/dJFmoYnJNPpBk/YJWo0FlQVERHnbXsvVWG9vNWajXae9n/NGX/w/+1Tf/GZ18BYtGjNvLlEV45kFR9djtEEUR" +
            "o1GANm7sDR2ytbtCqzFDGCRgDYPRPp3eDtbqsRUkqFZa+CrEOc291RUuXjyDlOqIk7w8f0plub77Ki9f+RmXFj/GT80UqSsP" +
            "m//R5/8BkycS8utdZkWdi4tnqEUBWmiMo4xJSIkYxysarToTEw3Asb3RhlxQWI0wFXCSgtFGLFq9EgDQElV4LrqTia4O8ymv" +
            "l2/Su59inrNU6wnNmTqra2s4LMY5dnu7dLr7dPZ7DHd6uPQ8wW7GzMk69RnB9v0Bclci24rRpkdH9xncFczPTKIz+NOvfoM/" +
            "+eY/pZ+tHloQTlikFFg7Tj851t4iCELiuEp/oEtRYzVFMWJ7ZwVPBTgKtMkfKGcgCqtUkvphktZOu01/WOD7PkVRPNQkySFI" +
            "zS5f+963+H/82ovM+Q021t7mf/jjV1hutnh6c472jXVqfoOlapV377/G+vA+iDJ6J5WHlGWzqJkT00TVmCI3bKzsEdiQvhkS" +
            "6QUKRgB3ApFkDqM9QDusTtzESiranYni9FS7cLRXyq0T10JOnFng2pVb+ConTnzSPCfNMrK0g167ytzSNNleQX835+zlWdzQ" +
            "Y3vTsnV/CKLg3fXr1PVjNE9U+Mo3/5o//ss/pJvdBQSBF+EJjzQbkersAdVFGVw5yAetxBV0rhmNhhiblxkSruwIVujRkQSu" +
            "0qWiVECtOoEQalwtD1mRc/P2yiHJHfIIygIpHNc3fsYbV1/n8VaLf/Xuj9hOr3FTJVR2z1DNu2xLn5V+yJqzFKLA93x8Lxj3" +
            "Mg0IwpDlM4soT9Hb7rO93iZxCevFNg0zx8i1s5DqLYEsHFbLsn2v1RU3u5PTuyesQuRVdrfbDDZSAJbPLlKtV1BS4awmiipY" +
            "Z8icZjMf8M6tO2xtZbTvadIVB5tw6lyd5mxCu5Bs70csTDb4+Tuv8T/92X/Pbv9WGWN2lB2ydH7oMzqaiiXHhxqtNb1uD8/z" +
            "qdXqVCstKvEEQZAccyU+sM6TuIofBA+VthpjWN9YKTP6Huokc+BGUmS2zQ/u/hUXTk9xeuYsoQqZcB7P1GN+7/c/y+c+cpqa" +
            "HZCR4yuPyI+J/YjIjwg8n+Zki4VT84BjfWWH4X7pvCsKj8g2GLC3VZPzqw6jgUIC2glXlL2UxTsZfcJ0jp1Bh/aNIdY5pucm" +
            "mV+aRSkPhQOjkVLgU7YWe211lVubIzwb8tJ3v8dP3/0R97dvsVA3PLU0x999/jIb2U/4n//6H7PWfwcnyj52URSW7gRdfs6j" +
            "KmMO3BjWWqIwpNWYIAoTqpUmU6155ueW8cYddA8cXVIp4riCp+SxqknIi3Tc3gACPyHwkwc5qQKEtLy981PW9jZ5ZvpZqrLJ" +
            "ZNyg2aqjTs1QbdWJhcSTHpWgSiWsEgdVAi/EkwEnzyxRb9UotOHu9Q3CPGSgBwTFDMYZCje8XmOm7XBFKf7hsIt44qauDuRO" +
            "J85n6Rea7Rttsp4mikIuPHmGOIjwfZ/A94j9EKtTPE8wt7CACy1pbnnz3St848df4Wtf/QYisVy8HHBv71X+2Uv/EzvpDaQ8" +
            "sEAseZ4dZj0LecyVjxj/bg/bTI5GI9JRfngSlUJRTSZpNeeJwyqVuEGrNUe12sL3QpwRZXr4keKMQhscEoFCa0OeZ2UWHeMG" +
            "sMLRN1t8+/p3ObtwjhO102QiwwUCL4wojMVKn1pQZ7l2jrOTzzCVLBL6MUm1woWnz+H7il57wPqdbWquSqcYUNfLDNxu4ZNc" +
            "8USYjQEod8BBF/G6O7GWis4NjIdIW6xtbLF/dwA4Tl9YZnpuBk+FVOMarfosyvcJVcHSRJUTy4pwpuCxM4+jC4+nn3uChaem" +
            "+fK/+2v+17/479kb3R5TWHxgUYU4lgvqKY8wDHE49DimfNiKylr6gz6BFzM5MU+rOUsS1YjCKkKosQV1rJDD6CMNPSz1aoNG" +
            "rXkQ0h8Dq3lt/Qd0TI9feez3eGzil9jY6XP9qy/xgzeucR9LEtS4OP80Ty58hPPzj1OJW5w4dYL55WkccP/6GtluXrbEyQIi" +
            "M0HPba61xNJNhzlkeu+gu58TLg9cta+c9+pI7F2ujE6q7eHrbL3ZYfKxKo2JGuefOs362jYjM8DYnDiOGfR79Ac7tKoht3f2" +
            "ufjYJ3j22ac4eX6BL//vP+EP/93/yu7wPmrcAcegcYzGvSseXRF54Mks83ssxhQH+Q848eA5MS75skcDYc6hhERJj2a1xX5/" +
            "57AezLmDzyqD+c458jwbN3wtd9RBfLmXrvKDqz/iP/+V/5Q7SZM31v+E7759g66U6CihKR3bo12oVOhmHaQH5586RaUakQ5y" +
            "rl+5R6Kr7OX7VPJTFG6EZvRGXczvOtxh72lvvANygcgdNm/Ypbd7cnV1Jn9yuZ9GrNzY5MTqNBOnfZ549izvvH6L1fspQpjx" +
            "Achy/fbreKs3CarP4TdO8WvLi7RfyentFHzx07+B8gW+8KlUY4Y25XuvfJ033/nrYwVzD06yRzvwHwS2S9PT4YceutBYox9S" +
            "3M496AQmpYdzMMqGDxV663H7S3H4/znSYohE4QUecRSTpXlZyCENr93/ITfXP0+lNoHbWaTrrbHncjznEHbEzd51tu02ga+Y" +
            "Pz3F2SdOlNVEtzfZvrfPhJukl3VY0KfYc3f2q2L6VSlUZtD5e3YAkCNcXnXzO21uvZy5/nIyOMVK5xpbr3aon4iZmmvx+OXT" +
            "bG9sEOqQyA8xsUbnmjTtkutVbqzu46U+T50O+fv/ySeoTkv8GIwR7LdHfOfbP2PQ7Y0jV+KRLQZKj2/Z1+HhfH+HKYoHTZge" +
            "UYxnjAGhcNaQjbqoo5WVVmPGIU0xtrAOPJiBH9Jqttjd3cVoi1KKdn6XP335T/hPP/ufcHnhk+wNhqTmDtazIB25GJI5j9Cv" +
            "8+RHnqDWrJGlOW+/dps4rdAp9omyE2AFA7d75ZT8+F2Lycdt8XOgUF+69Af1g84dDudJpGeFTgdy+6mGPlXtBiuo1DBzaoJk" +
            "IqTaqLJyc4V+b3CYEm4cxNVpKEZMNE5ArUFlwtGaFlQaHr1eyl994xX+yf/8R3z5W3/CducuUrpDsXG8DlhJNc6EC/CUP35u" +
            "3EX9IaI/XC98sBOULKMEHIvRl+0wB+DMWDGLcUBfHoJ34Oougy6OzmiDld09ZlvnWZp5ktBvIZXCDxVB4BP6ISfPL/FLv/oC" +
            "QeRx5+oab7x0g1pWY23YYXr4Ubp2bSiE/PKEPH3XYo62PM68Iz3wC4HInLBZw55c7akf/Th3w9+u9M9zt/0OSz+Zp7IQMjHd" +
            "4Llfeoad7U7ZFU2FKD9HZznGai6dtlx43mduJkJoww9eeps//z++wctXXqKbrWKdRkqLkB7W6jLt/FilS5nOV2U0slhhxj53" +
            "Od4ZAik9oGw5c0B0IdTYtA1Kk9aYg3qaw4C4NmWrfynHXvyxzBfj2HGapkg5jnbJ0tQUynK982P23ljnqeWP8dHlF3jCP8/9" +
            "3l329QZRXfD8py5TbVRIByPe/PE1kmGFvaJNkp5EWJ+u23hjWb5w1aKzo9wPaPWlS39QOdo10eGURCknbL8n1y419Zlaz9vC" +
            "DAfMzE1SmQ1oTjbY2+7R3u6jvACTa3SRk8RLPPfUJRbmK6zc3ORP//jP+dOv/O9cu/8y2nZBWJRUeL6HNmUnrIOqRzGeEXCQ" +
            "15mmKSDxvQg19rE4IAgiJpqTFEU+bkmpxxzr43mSOI6OVMoc7JyytizLh1ibl0Q/AOFYo+8SAIWnPHzlj0OLPkpp9rM1Ngfr" +
            "BEHAiYllGskE5y4v8/THz6M8yduv3ebaj+6TFDEb/QEzo+dp27s9JcJ/NSlP3bVlm+OjDb9z9aVLfxAd7ZooEArhvMg1i315" +
            "11cuvBTrWbGtblAdJkycrxPXQ5qtJvdvr5P1M5SoUI/OcfHEszSrId/9zo/4l1/9M67c/htS00YK+2AIAw8a6R1GkNwDExDx" +
            "QCRJKVEq5DCRQCikUCRhlWIsyx8470oHXqE1xo2zEQQ4VzYP0TrHOY0QblwjVhJcCTlu+K0Oie8rH095ZZ6s7xP5EXEUE8cJ" +
            "ni9JbZeh7rF4aoaP//qTJPWAvc19Xvr661Q6NdZHW1QGF1E6Ztfd/Jsl+ZHvCiF64x7TBwDkBwCEx7shjbuCK4+401G3Tjf0" +
            "yelC5Oxnq0z6LRqnE2rNCoEXsrdaMFs9x8nJJbrdTb7/6jd549YPGOSbZaLXYdrGsRrgw6yFwzrSw7o6hKBarRJFEXmmHzTk" +
            "Vj6+l9Ad7KOLYnyfctLYWNhrWw51qNdrZFmKLnKs1RRFhrPFoXWllBo3+z6Q92pcAFJyv+d5+J5PGEZEYUIcVajEFapxlTiO" +
            "qDZCXvziUyyemUFnlh//1Rvsv5tS5AX9QcRU+jSb7u31mpj7s5qc23APuH94ZAKHVl+69AfB8VlgAqGccCp0NTMUu4NM9J5u" +
            "FmeDPXUf3RkxPdsimQ6YnGkQkNDf3ufm5ltc336dfrYxBvdBB8OHcmbH/YAOBi884Hx32D5GCIm1jjwvcEIQRzUCP0Qi8T2f" +
            "VmOC+aklur02DjuumCnB9MaEdRaUkARBhJCSLBugdVbGupWiUqmAe7DTgiAg8AOUKH37ge8RBRFxmBBFFZK4RiWqUokq1OIa" +
            "v/QrH+OZj11CSMnV1+/w1vdvU8mqrAz3mB18jL7d00PaXzkhn3t1TPz+mPhHu6vrg66J7+kJViaVOC9hqrsnr8eBq16I9azY" +
            "EtcJ2gFTpxtETZ/adMSNO++ysnYLgT6sTHTiaB6Ye6/P7Ij144BqXCcOE/I8A0TZS85ZnJMEfkzghw+Zjb5SZEWKsaVfSQgI" +
            "ggAlJYybsgoBWT6g19tD6+whWc84gnawA5I4IfADBALfDwmCiChIiKOEJK5RjaskcZUkqvDMi0/yqS+8SBD5bKzs8jdff51q" +
            "u8HKaINq7xK+rrPp3nl5UTz7554I98fDHgaPmLZhDhq3cmwaXimKhJPSKeERbe/JGyfrennGOY+N/Ab1YZ3mmQpRLWB6vsXW" +
            "+i79/SHj9PyjXmXkkVp4eSSj+TB3Bw7duXmeopQiisZVMHZcHa8cWT4izXoM0y6DUR+lJJ4qUyjLctSgbKVgXdnBpd9mMOzg" +
            "nClFzpjYUqhxConCk6WyHWsF/MAnDEKiKCEOKyXXJxUqcZUkqvL4k2f5ld/6NNVGQrc95KWvvQJ3PPbyNqY3TSt7gg13Za3K" +
            "zL9oyqVVV5qdgyMjTx6aN3O0d/R7BrUJhHTCydDVdS76uwOx+UQzP18Zih47vftMuCa1kzHVZoW5uRnW13YY9UaHybLjo9aD" +
            "jzxI/D2SUXbwWJuCLB+VMng8mMcaO36/wbmc0ahLUaTk+RAhHUlSRUlFGAZlPmeej81USbe7RaFHD3G953lUksqYEWRZMO55" +
            "+J6H5wcEfkQYRMRhTBImVOIq1aRGNaoQhzHnHjvJF37vs0zMNEkHKT/4y5+z/2ZGlme0e5K50YvsuttDTfpni/LyG8dEz+g4" +
            "9x8AID9gTGF5BBJOJna631X3h1pkF1vZeb8j1mnvbDEZTFE9EVCfrDA7N832xi7DXvqgp8PBIUqKBzk0h4ef8dQLIVBybBqO" +
            "7XJdmBI6WRLUU/44PlwScnpmmjisMhqliHFgxRiN5/kopRil3dIvpEq/kJIST5QTNXzPR46bPpXyPiAMAqIgIgpjkqhCJa6V" +
            "XB9XScIK5x4/xRd+75eZXpikyAwvf+8t1n7SxmWW9cGQucEnGNq22Xer/+6kePE7Qoi+w/WPcP9Dsv9gtsz7TdB46PfSBYas" +
            "uLntXXlNSOedb2Rn1Y5Yobe5x1TYIl4MaEzVWFico73dLcXRmKgHO+JAwUoh8KRPLWlQ2Lw8ZMkHp2ExVta1SrMMz497Uhzo" +
            "jYN8UKUijDZ4yj9U+r4XIIA07SEl+H5ApVLBGVDSG8+akaWd7wUEfkgUhIRBRBImJHEpbipRlUpcIY4SHn/6HF88IH5e8MoP" +
            "3uXOS9uokeT+oM1s/+NoY9yWe/evl8RHvuKLpPMI4j9y1NXxHfDIgT4CIZxwQjpF4qbWdrx3Ys9WztSyk2KL2+yvd5j0J4kX" +
            "AuqTVRZPLTDoj+jt9kCUhxvf88ZgSKSEwIs5tXCW/X5nnGgrxjb5QXqfJAqjcQ9QU8prpVCqBDDPC0I/QQg1PqiVycBBEJaT" +
            "kdJ+acuPFasUY6KPTczAD0pFG47lfVQSPzng/KhCtVLh8otP8/nf+jSt6SZ5pnntb65y5/tbyIHk3mCXqf6LoH02uPLqnLj0" +
            "pxUxte2c7QvxkM3/HtFztHe0+JDBnYdJ3044oVxgQ9dY2fHergW2cbKanhKb7h67a1u0aFBdDKk2E06ePYFA0t7ax1nGgRE1" +
            "BqCchtru7SCEPZz/VYqiMsNAKonWBc6Wp+dKUqE1MYHnqbLRmHPEcZmZ7ckH0ZwwjMBZdJHSbDSIwghd6FLZ+n5p4fjhoayv" +
            "xhXqtTpRkFCJSu6Po5jWZItP/soLfOoLL1KtJ6SDjJ99723uvrSHHmhWBm0mey8ii4h1fn5lgrP/vCmWVi1mMCb+o7jfHJ8t" +
            "9kE74D1gHIAQuEQHrnZn13u7FtjaUi07LXbcKpvrq1SHNWoLMVHDZ/nsCRqTdTq7XbJhgRIPDjtqfPL0hIdU45PoEUtFHTrE" +
            "ZJnZFviEQUC9Vsdog9GWOKqUU5LGwAogjmJMkaF1WmYnu1L3+EFIMBY1YRiTRAlJWCGJK8RhtRQ/UYU4TDh5eolf/a3P8vSL" +
            "TxCEHt32kJ/85Vvs/LTHaDhkYzBkuvdx0N6Y+Kf/eEKcvmfRgyOn3aMHrvebN8nxHfChXXjEOIUkcJUidPXbu+rdUNn4ZCM9" +
            "Lzt2l/vbN/E3Q5rTVYKWx+ziNMtnF7Eaevv9w9RtTx7kUvp4So3X+HfplSdTqZDKHzvfoCg0aZphx8lacZjg++GhkgZHEiZj" +
            "+V+alUp6RH5EFIREYUwcJofETqIqSZQQhzFRENNqtvjoJ57hV377k5w4vYAQgo17u/z0a2/Tv5KzO2rT7ktme5/AGO02eevV" +
            "Sc7+8yPE7485/4OI747vgA+S/+/XOsY5YfFdRSdu6tauumod9nQzfczLtOVu911Gd3LqYYN42qPaqnD6wkmmZydIhxnZIC9H" +
            "Dno+nucfEt73/BIAr3SCqfHV83yU55WF2WOglOcRhhGBH6GUR+D7pWnpeRidjxVrSDTm+NKdMCZ8/GBFQUylWuX8xbN87u9+" +
            "io984ikq9YQi11x97S5Xvn6H0d2C1XSLojfFzOB5BrZjdrj61zNc/NOGWFo9Qvzh+1g87ztL7BcZ5Oa9zyC3yni+QMWIvL6p" +
            "XvukR/y7E+b8RBF26DWuMNMIufzsRZY/NUM0Wx43+t0B775+nZ+//DbrKxuk6agsCtGaXOfkWmN0cWTqkR7PfdGHDbkPZwyo" +
            "srGHkh5KKfI8RwiD0cURUTcGUZWOtcMcHuURxzGLJxd55mMXuXDpDJVqgrWW3c0O7/7NXfbeGNAZdNlK+9R6F6lmS+y5W8M+" +
            "O99Y5PJfRqK+Zx8ctAYfMFvSvM9IQ3d0luRRIOQjBrq93zS95HCanrpyKRf935+w5y/4IqJbextR2+bC/DJPvniOycs1VK2s" +
            "KevtD7j5zh2uvPYua/c2GPaH5dA1a9C6OJx4dDjMx5oHbekPquSdQCmfMEzwPI/RqF8q9XE3FTVOG/cPU8c9PBVQrVdZOrPA" +
            "pcuPcfrCMpVamV807I+4/cYaKz/eobcxYjPfJR9UmRg8g9AeW+KdNefcV07wkZ9K4feOOdiGHzBd1b7fVKXjAPAIENSxESf+" +
            "sdnBhyAIVNyTqwttdfOLFTv7qYZdivNwj179XapVy8WTZzn/whKNx2NURYyrRzLW721y453b3L1xn93tNumwHGd40O/fHe3/" +
            "f+BqPrgCcVTD93yGo+6hJVVaXKUu8T2POImYnpvk1Pllzj5xmtnFKYK4bHuQDnNWr2+z8tMtOndG7Iz22E811f4FqtkSXTZ1" +
            "mzuv1ln482l34bYT9ugwz+GxGcPFL0r8Dxtne3Sp9xn0c3ycbSKdSrRIa9vqrcsW/Xea9tSZkJoYJiukldu0ah7nl09x5tlF" +
            "mo9X8Bqy7AlhLIPekO31HVbvbLB2f5Pd7T363QF5mmMOhjmMM90O/w4HnheiPI+iSMd9hHziJKbWqDMx02R+aZrF5Rmm5yaJ" +
            "q/E4B9Uy6GVsXt9j4/U99u502Rm06eQZwWCJRnoObQr2xM0NTfaXc+6pH0Y0206YkcMNjwVWsl9Q5n/oONsPAuH4iEPv2CTt" +
            "eDxJOxlP0o57cnWuI29/OqD66YY5OamkxzC+T1q5R7UiOD17gtOPLTD9eIN43kPEY++ogyIvGPRHdDt9uns9uu0u/e6A4XBE" +
            "kRfowmC0Qfml4g3CkCSJSRoJjVad5mSTerNCXInKs8M40J9nBd3NATvXumy922Fvvctu2qGfabzhPPX0DBiPDvf6A7H9k7o7" +
            "8e0pd/auE4wcdvSIEbbZBwx0tn/bgc5/GxAeNVX7ocna0qnYYpK2unlqKLY+HbnW8zW72FDSJw03GSX38KIB07UGS/PzzJ+Z" +
            "pnmqSjLjo6qi/PSDmO04kFOONC+TgcpRVAddccuUxHFcZ+yRtRjjKIaa/nZK516f9u0eu6sd9rr7dPIheR4SjRappidwVtBl" +
            "ddgXW29GrvHdaR6/6rmw74RNx7PlH0X4o4csfYTwH0r8DwLgg3TC+82V94/0xj+YKx+Nd0NoRFZpy1unR3L344GrPle185MB" +
            "NWG8Pmm0Th5t44c5jThmujXB9PQkzbkatemYsOnhVxUyEuCXlZZjX16ZE2rLoLrVDpMaiqEh7RQMd1J6W0P2N/t0dvfpDLv0" +
            "i4y88PCySeJskaBoUrgRPda7I9F+M3TVH05y4Wrgqr3xGPP0COGzR8j6o3b+32qe/IcB8EEjzo/qheNWkn98R4yBiAQqMKJI" +
            "uvLeQl9uPCvxLid2cjl2k5EiwHpDsmCHLNjF+QN83xL5HtUwohLHJHFMGAVlkNwr589b47DGogtDluZlFecoZZSljPKUkS4o" +
            "jEIUCUHeIsyn8XW9HFki9ooB2+ua0c8TN/Vqi9N3A5cMLCYbE/6A6AfWTXaYQ/WwrH8/kfOBxP9FAHg/EI5P1FEfAsSD2TRO" +
            "hGLcvHood5s9uXI6F/1LygWPRa45H7pm4rsYIQVWpmhvgFY9tDfAygyninFTkXGDzwejkXFWgPORJsQzFTxdxTNVlInBCjQp" +
            "qehkI9HeKhhd91x0peFO3Ky62V2Byhwmd8IdpI4cXfkxjtfvw/X2fQZ38kEAyA970fs46d5v+vajRFPw8HIlKE4GAuk7TDCS" +
            "e42B3FzIRPeUw532XLjgu8qET5J4LvaUC5Cow0ga4kGfhvJsrg7NU4fFkFOI1BQMRrkYdDSjDeCO7yq3qm52NXFTbY8ws5gC" +
            "4XKHO5qxlh+T70etm6MWjjk+mO1vQ3xAeL/Ai3iojOSDBxYfVUDmiGI62LZjQETZP1+4wKJ9gfBjO9lP7PQmcEWLNMxEt5qK" +
            "dnModqaMzKfATjhcQzqvIlGBFSZ0GN8BwkkthZc5bG7JhiD3Qbal83cDV9mpufm9yDV7voszgSwsRiNcYSgOspQPcjUfxelH" +
            "FexRwtsPmZT6C9HVO8bZ7hcA4vjr7ENNcR4NhD7yx/hHwRAlE/gI5zuM53Cecr6XuKluhZkNgVTgpMUoKwrfUChL4RlReLas" +
            "bxASZaXztcLXEk8rF2iBMgJpSvVsDDjjhNUOe/A99EGC7NHswGOcflzGP3Ic4d+S6x+i9f8f9rLSm9HJ3bsAAAAASUVORK5C" +
            "YII=";
}
