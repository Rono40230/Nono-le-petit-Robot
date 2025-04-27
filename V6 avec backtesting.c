//@version=6
strategy("Nono le petit robot", overlay=true, default_qty_type=strategy.fixed, initial_capital=1000, currency="USD", max_bars_back=5000)

// Définition des types
type fairValueGap
    float top
    float bottom
    int bias
    box topBox
    box bottomBox
    int barIndex
    int time
    int creationBarIndex
    string imbalanceType
    bool isVisible  // Nouveau champ pour gérer l'affichage
    int age         // Nouveau champ pour suivre l'âge

type orderBlock
    float top
    float bottom
    int bias
    box box
    int barIndex
    int age         // Nouveau champ pour suivre l'âge

type IndicatorProps
    string name
    color labelColor
    color textColor
    string baseText

// Déclarations globales
var IndicatorProps[] indicatorPropsArray = array.new<IndicatorProps>(0)
var int lengthSMA20 = 20
var int lengthSMA5 = 5
rangeSize = high - low
avgRange = ta.sma(rangeSize, lengthSMA20)
fvgAvgRange = ta.sma(rangeSize, lengthSMA5)
length = lengthSMA20
var float trendSMA = na
avgVolume = ta.sma(volume, lengthSMA20)
var float valeur_pips = 0.0
var bool useMaxDailyLoss = false
var bool useMaxDailyGain = false
var float maxDailyLossPercent = 3.0
var float maxDailyGainPercent = 3.0
var float timeframeSeconds = timeframe.in_seconds(timeframe.period)
timeframeSeconds := math.max(timeframeSeconds, 1)
var float resistanceLevel = na
var float supportLevel = na
var line resistanceLine = na
var line supportLine = na
var float supportLevel10 = na
var float resistanceLevel10 = na
var line supportLine10 = na
var line resistanceLine10 = na
var label supportLabel10 = na
var label resistanceLabel10 = na
var float supportLevel50 = na
var float resistanceLevel50 = na
var line supportLine50 = na
var line resistanceLine50 = na
var bool squeezeOn = false
var bool squeezeOff = false
var table mainTable = na
var int tableNumRows = 16
var int tableNumCols = 3
var float mom = 0.0
var bool squeezeOnStart = false
var bool squeezeOffStart = false
var label supportLabel50 = na
var label resistanceLabel50 = na
var orderBlock[] orderBlocks = array.new<orderBlock>(0)
var float supportLevel100 = na
var float resistanceLevel100 = na
var line supportLine100 = na
var line resistanceLine100 = na
var label supportLabel100 = na
var label resistanceLabel100 = na
var float supportLevel250 = na
var float resistanceLevel250 = na
var line supportLine250 = na
var line resistanceLine250 = na
var label supportLabel250 = na
var label resistanceLabel250 = na
var int srDisplayPeriodBars = 40
var int lastSRSignalBar = na
var bool srLongCondition = true
var bool srShortCondition = true
var bool srSectionExecuted = false
var float closestLevelPrice = na
var bool isSupport = false
var bool isConfirmed = false
var int last7Days = 7 * 24 * 60 * 60 * 1000
var int lastSqueezeSignalBar = na
var int lastCandlePatternBar = na
var int lastFibEntryAdviceBar = na
var int lastOBBar = na
var bool in_trade = false
var float tp_level = na
var float sl_level = na
var float currentCapital = 0.0
var float riskAmount = na
var int supportBarIndex10 = na
var int resistanceBarIndex10 = na
var int supportBarIndex50 = na
var int resistanceBarIndex50 = na
var float longTermSMA = na
var int supportBarIndex100 = na
var int resistanceBarIndex100 = na
var int supportBarIndex250 = na
var int resistanceBarIndex250 = na
var int consecutiveWins = 0
var int consecutiveLosses = 0
var int currentDay = na
var float dailyPnL = 0.0
var bool dailyLossLimitReached = false
var bool dailyGainLimitReached = false
var float entry_price = na
var float entry_capital = na
var float lot_size = na
var int entry_time = na
var line entryLine = na
var line slLine = na
var line tpLine = na
var label entryLabel = na
var label slLabel = na
var label tpLabel = na
var int tradeStartBar = na
var float tradeEntryPrice = na
var float tradeTP = na
var float tradeSL = na
var float trailingStop = na
var bool trailingActive = false
var int pyramidPositionCount = 0
var float stopDistance = na
var fairValueGap[] fairValueGaps = array.new<fairValueGap>(0)
var float topFVG = na
var float bottomFVG = na
var bool lastBullishFVGDetected = false
var float currentATR = na
var float atrAverage = na
var bool lastBearishFVGDetected = false
var int lastBullishFVGBar = na
var int lastBearishFVGBar = na
var bool lastBullishOBDetected = false
var bool lastBearishOBDetected = false
var int lastBullishOBBar = na
var int lastBearishOBBar = na
var float fibHigh = na
var float fibLow = na
var float fibRange = na
var float trendAverage = na
var float fibLevel1Price = na
var float fibLevel2Price = na
var float fibLevel3Price = na
var float candleVolatilitySma = ta.sma(high - low, 14)
var float bbWidthSmaPrecomputed = ta.sma(candleVolatilitySma, 14)
var int testCountSupport10 = 0
var float squeezeVolatilitySma = ta.sma(high - low, 14)
var int testCountResistance10 = 0
var float fibLevel4Price = na
var float fibLevel5Price = na
var bool nearFibLevelBullish = false
var bool nearFibLevelBearish = false
var line fibLine1 = na
var label fibLabel1 = na
var line fibLine2 = na
var label fibLabel2 = na
var float stochK = na
var float stochD = na
var line fibLine3 = na
var float trendScore = 0.0
var box slBox = na
var box tpBox = na
var label fibLabel3 = na
var line fibLine4 = na
var label fibLabel4 = na
var line fibLine5 = na
var label fibLabel5 = na
var box fibBoxBetween50and618 = na
var box fibBoxBetween618and786 = na
var int lastFibLabelBar = na
var string trendStrength = "Inconnue"
var string trendDirection = "Neutre"
var color trendColor = color.gray
var float macdLineNormalized = na
var float signalLineNormalized = na
var float adxNormalized = na
var float diPlusNormalized = na
var float diMinusNormalized = na
var float obvNormalized = na
var float obvMANormalized = na
var bool isBullishTrend = false
var bool isBearishTrend = false
var label[] stmaLabels = array.new_label(0)
var float stmaHMA = na
var float stmaUp = na
var float stmaDn = na
var int stmaTrend = 1
var float stmaUpPlot = na
var float stmaDnPlot = na
var float stmaMPlot = na
var float fibClosestLevel = na
var string fibClosestLevelStr = ""
var float fibClosestLevelValue = na
var bool fibIsUp = false
var string candlePattern = ""
var bool candleIsUp = false
var int lastSignalBar = na
var int lastSignalDirection = 0
var int lastBreakoutBar = na
var float lastBreakoutLevel = na
var int lastBreakoutDirection = 0
var int lastRebondBar = na
var float lastRebondLevel = na
var int lastRebondDirection = 0
var int backtestTradeCount = 0
var float backtestTotalTradeDuration = 0.0
var float backtestAverageTradeDuration = 0.0
var float backtestProfitLoss = 0.0
var table backtestMetricsTable = na
var int winningTrades = 0
var int losingTrades = 0
var float totalGrossProfit = 0.0
var float totalGrossLoss = 0.0
var float maxDrawdown = 0.0
var float peakCapital = 1000.0
var float currentEquity = 1000.0
var float upperBB = na
var float basis = na
var float lowerBB = na
var float upperKC = na
var float ma = na
var float lowerKC = na
var bool initialUseSTMA = false
var bool initialUseRSI = false
var bool initialUseStoch = false
var bool initialUseADX = false
var bool initialUseMACD = false
var bool initialUseOBV = false
var bool initialUseSqueeze = false
var bool initialUseCandlePatternsInConditions = false
var bool initialUseFVG = false
var bool initialUseFibonacci = false
var bool initialUseSR = false
var bool initialUseOB = false
var bool initialUseDivergence = false
var bool initialUseBreakout = false
var bool breakoutResistanceCross = false
var bool breakoutSupportCross = false
var string lastImbalanceType = ""
var int lastConsecutiveImbalanceBar = na
var int lastLiquidityImbalanceBar = na
var float cachedSupportLevel10 = na
var float cachedResistanceLevel10 = na
var float cachedSupportLevel50 = na
var float cachedResistanceLevel50 = na
var float cachedSupportLevel100 = na
var float cachedResistanceLevel100 = na
var float cachedSupportLevel250 = na
var float cachedResistanceLevel250 = na
var float cachedSupportLevel = na
var float cachedResistanceLevel = na
var float cachedRsiValue = na
var float cachedStochK = na
var float cachedStochD = na
var bool isNewBar = false  // Pour détecter une nouvelle barre
var float priceLow = na
var float rsiLow = na
var float priceHigh = na
var float rsiHigh = na

// Paramètres pour le backtesting
backGroup = "⏳ Fonction de Backtesting ⏳"
enableBacktest = input.bool(false, "Activer le mode backtesting", group=backGroup, tooltip="Active le mode backtesting pour tester la stratégie sur des données historiques. Désactivez pour le trading en temps réel.")
showGraphicsInBacktest = input.bool(false, "Afficher les éléments graphiques en backtesting", group=backGroup, tooltip="Affiche les lignes, étiquettes et tableaux en mode backtesting. Désactivez pour améliorer les performances.")
applyDailyLimitsInBacktest = input.bool(false, "Appliquer les limites quotidiennes en backtesting", group=backGroup, tooltip="Applique les limites de profits/pertes quotidiennes en mode backtesting.")
usePyramidingInBacktest = input.bool(false, "Activer la pyramidalisation en backtesting", group=backGroup, tooltip="Active la pyramidalisation en mode backtesting.")
useTrailingStopInBacktest = input.bool(false, "Activer le trailing stop en backtesting", group=backGroup, tooltip="Active le trailing stop en mode backtesting.")
backtestSpreadAdjustment = input.float(0.05, "Ajustement des frais pour spread (%)", minval=0, maxval=1, step=0.01, group=backGroup, tooltip="Ajuste les frais de trading en mode backtesting pour simuler le spread (par exemple, 0.05% pour 5 pips).")

// PARAMÈTRES GÉNÉRAUX
genGroup = "📌 Paramètres généraux 📌"
start_hour = input.int(0, title="Heure de début (0-23)", minval=0, maxval=23, group=genGroup)
end_hour = input.int(23, title="Heure de fin (0-23)", minval=0, maxval=23, group=genGroup)
pair = input.string("BTCUSD", title="Choisir une paire", options=["BTCUSD", "XAUUSD", "USDCAD", "USDJPY", "GBPUSD", "EURUSD", "CHFJPY", "GBPJPY", "EURJPY", "EURGBP"], group=genGroup)
trade_fees = input.float(0.01, title="Frais de Trading (%)", minval=0, maxval=1, step=0.001, group=genGroup)

// CAPITAL INITIAL & GESTION DU RISQUE
riskGroup = "💰 Capital initial & gestion du risque 💰"
startingCapital = input.float(1000, "Capital initial ($)", minval=100, group=riskGroup)
currentCapital := startingCapital
risque_par_trade = input.float(1, title="Risque engagé par trade (%)", minval=0.1, maxval=10, step=0.1, group=riskGroup)
riskAmount := risque_par_trade
useMaxDailyLoss := input.bool(false, "Activer la limite de perte quotidienne", group=riskGroup)
maxDailyLossPercent := input.float(3, "Choisir un % maximum de perte par jour", minval=1, maxval=10, step=1, group=riskGroup)
useMaxDailyGain := input.bool(false, "Activer la limite de gain quotidien", group=riskGroup)
maxDailyGainPercent := input.float(3.0, "Choisir un % maximum de gains par jour", minval=1, maxval=10, step=1, group=riskGroup)

// PARAMÉTRAGES DES TP & SL
tpGroup = "🎯 Paramétrages des TP & SL 🎯"
sl_ratio = input.float(1.0, "Ratio SL", minval=1.0, maxval=5.0, step=1, group=tpGroup, tooltip="Ajuste la distance du Stop Loss. Par exemple, 2.0 signifie que SL = pipsDistance / 2.")
tp_ratio = input.float(1.0, "Ratio TP", minval=1, maxval=5.0, step=1, group=tpGroup, tooltip="Définit la distance du Take Profit. Par exemple, 2.0 signifie que TP = 2 * pipsDistance.")
useTrailingStop = input.bool(false, "Activer Trailing Stop", group=tpGroup)
trailMultiplier = input.float(1.0, "Multiplicateur pour Trailing Stop", minval=0.5, maxval=5.0, step=0.1, group=tpGroup, tooltip="Ajuste la distance du Trailing Stop par rapport au prix actuel.")
trailActivationPct = input.float(1.0, "Activation Trailing Stop (% profit)", minval=0.1, maxval=10.0, step=0.1, group=tpGroup, tooltip="Définit le pourcentage de profit requis pour activer le Trailing Stop.")

// PYRAMIDALISATION
pyrGroup = "📈 Pyramidalisation 📈"
usePyramiding = input.bool(false, "Activer la Pyramidalisation", group=pyrGroup)
maxAddPositions = input.int(3, "Nombre max de positions supplémentaires", minval=1, maxval=10, group=pyrGroup, tooltip="Définit le nombre maximum de positions supplémentaires à ajouter.")
pyramidMinProfit = input.float(1.0, "Profit minimum pour ajouter une position (%)", minval=1, maxval=5, step=0.1, group=pyrGroup, tooltip="Définit le pourcentage de profit minimum requis pour ajouter une position.")
pyramidSizeReducer = input.float(0.8, "Réducteur de taille par position ajoutée", minval=0.1, maxval=1.0, step=0.1, group=pyrGroup, tooltip="Réduit la taille des positions supplémentaires ajoutées.")

// INDICATEURS
indGroup = "📊 Indicateurs 📊"
showIndicatorTable = input.bool(false, "Afficher le tableau des indicateurs", group=indGroup, tooltip="Active ou désactive l'affichage du tableau des indicateurs.")

// Paramètres de filtrage des indicateurs
filtGroup = "Filtres pour le trading"
volatilityPeriod = input.int(7, title="Filtre 1 : Période de l'ATR", minval=5, maxval=14, step=1, group=filtGroup, tooltip="Définit la période de l'ATR pour mesurer la volatilité. Par défaut 14.")
volatilityThreshold = input.float(1.0, title="Filtre 2 : Seuil de volatilité", minval=1.0, maxval=3.0, step=0.1, group=filtGroup, tooltip="Seuil minimal de volatilité (en multiples de la moyenne de l'ATR sur 50 barres). Par défaut 1.0.")
useSignalFilter = input.bool(false, "Filtre 3 : Activer le filtrage des signaux contradictoires", group=filtGroup, tooltip="Active le filtrage pour éviter les signaux contradictoires (par exemple, un signal haussier suivi immédiatement d'un signal baissier).")
signalCooldownBars = input.int(6, "Filtre 4 : Période de cooldown (barres)", minval=5, maxval=7, step=1, group=filtGroup, tooltip="Nombre de barres après un signal pendant lesquelles un signal contradictoire est bloqué.")
momentumThreshold = input.float(50, "Filtre 5 : Seuil de Momentum (RSI/Stochastique)", minval=40, maxval=60, step=1, group=filtGroup, tooltip="Seuil commun pour confirmer le momentum avec RSI et Stochastique. Par défaut 50.")
signalThreshold = input.int(5, "Filtre 6 : Seuil de validation des signaux", minval=1, maxval=8, step=1, group=filtGroup, tooltip="Seuil minimum pour valider un signal d'achat ou de vente (vote majoritaire et trendScore combinés). Par défaut 1 pour faciliter les trades.")

// SuperTrended Moving Averages
stmaGroup = "-- SuperTrended Moving Averages (SuperTrend)"
useSTMA = input.bool(true, "Activer SuperTrend", group=stmaGroup)
stmaLength = input.int(9, "Longueur de la Moyenne Mobile", minval=6, maxval=21, step=1, group=stmaGroup, tooltip="Définit la période de la moyenne mobile HULL. Une valeur plus élevée rend la moyenne plus lisse mais moins réactive.")
stmaPeriods = input.int(10, "Période ATR", minval=7, maxval=21, step=1, group=stmaGroup, tooltip="Définit la période pour le calcul de l'ATR, utilisé pour déterminer la volatilité et ajuster les bandes SuperTrend.")
stmaMultiplier = input.float(1.5, "Multiplicateur ATR", minval=1, maxval=2.5, step=0.1, group=stmaGroup, tooltip="Définit le multiplicateur appliqué à l'ATR pour calculer la distance des bandes SuperTrend par rapport à la moyenne mobile.")
longTermTrendLength = input.int(50, title="Période de la tendance à long terme (SMA)", minval=20, maxval=100, step=10, group=stmaGroup, tooltip="Définit la période de la moyenne mobile pour identifier la tendance à long terme. Par défaut 50 pour le scalping.")

// RSI
rsiGroup = "-- RSI"
useRSI = input.bool(false, "Activer RSI", group=rsiGroup, tooltip="Active l'utilisation du RSI dans les conditions de trading.")
rsiOverbought = input.float(80, "Seuil de Surachat", minval=70, maxval=90, step=1, group=rsiGroup, tooltip="Définit le seuil de surachat pour le RSI. Un signal de vente peut être généré lorsque le RSI dépasse ce seuil.")
rsiOversold = input.float(20, "Seuil de Survente", minval=15, maxval=30, step=1, group=rsiGroup, tooltip="Définit le seuil de survente pour le RSI. Un signal d'achat peut être généré lorsque le RSI passe en-dessous de ce seuil.")
useDivergence = input.bool(false, "Activer la détection de divergence", group=rsiGroup, tooltip="Active la détection des divergences entre le prix et le RSI pour confirmer les signaux de trading.")
divergenceLookback = input.int(25, "Période de recherche de divergence", minval=10, maxval=50, step=1, group=rsiGroup, tooltip="Nombre de barres à examiner pour détecter une divergence.")
momentumPeriod = input.int(9, title="Période commune pour RSI, Stochastique, MACD", minval=5, maxval=14, step=1, group=rsiGroup, tooltip="Définit la période commune pour le calcul des indicateurs de momentum (RSI, Stochastique, MACD). Une valeur plus élevée rend les indicateurs plus lisses mais moins réactifs.")

// Stochastique
stochGroup = "-- Momentum"
useStoch = input.bool(false, "Activer Stochastique", group=stochGroup, tooltip="Active l'utilisation du Stochastique pour confirmer le momentum.")
stochSmoothK = input.int(3, "Lissage %K", minval=1, maxval=5, step=1, group=stochGroup, tooltip="Définit le lissage de la ligne %K.")
stochSmoothD = input.int(2, "Lissage %D", minval=1, maxval=5, step=1, group=stochGroup, tooltip="Définit le lissage de la ligne %D.")

// Average Directional Movement
useADX = input.bool(false, "Activer ADX", group="-- Average Directional Movement")
adxThreshold = input.float(17, "Seuil ADX", minval=15, maxval=20, step=1, group="-- Average Directional Movement", tooltip="Définit le seuil à partir duquel une tendance est considérée comme forte.")
minDIGap = input.float(1.2, "Écart min DI+/DI- (x stdev ADX)", minval=1, maxval=1.5, step=0.1, group="-- Average Directional Movement", tooltip="Écart minimum entre DI+ et DI- pour ADX, en multiples de l'écart-type d'ADX. Par exemple, 0.5 exige que DI+ > DI- + 0.5*stdev(ADX) pour un signal haussier. Augmentez pour confirmer des tendances fortes.")

// Moving Average Convergence Divergence
useMACD = input.bool(false, "Activer MACD", group="-- Moving Average Convergence Divergence")
macdStdevLength = input.int(25, "Stdev période MACD", minval=20, maxval=30, step=1, group="-- Moving Average Convergence Divergence", tooltip="Définit la période pour le calcul de l'écart-type du MACD, utilisé pour déterminer l'écart minimum entre la ligne MACD et la ligne de signal.")
minMACDGap = 1.2

// On Balance Volume
useOBV = input.bool(false, "Activer OBV", group="-- On Balance Volume")
obvLength = input.int(5, "Période OBV", minval=3, maxval=9, step=1, group="-- On Balance Volume", tooltip="Définit la période de la moyenne mobile pour l'OBV.")
minOBVGap = input.float(0.7, "Écart min OBV (x stdev OBV)", minval=0.4, maxval=1.2, step=0.1, group="-- On Balance Volume", tooltip="Écart minimum entre OBV et sa moyenne mobile, en multiples de l'écart-type d'OBV. Par exemple, 0.5 exige OBV > OBV_MA + 0.5*stdev(OBV). Augmentez pour des signaux de volume plus robustes.")

// Squeeze Momentum Indicator
useSqueeze = input.bool(false, "Activer Squeeze", group="-- Squeeze Momentum Indicator")
bbLength = input.int(14, "Longueur BB", options=[10, 14, 20], group="-- Squeeze Momentum Indicator", tooltip="Définit la période des bandes Bollinger pour le Squeeze.")
bbMult = input.float(1.5, "Multiplicateur BB", minval=1.4, maxval=1.6, step=0.1, group="-- Squeeze Momentum Indicator", tooltip="Ajuste l'écart des bandes Bollinger en fonction de la volatilité.")
kcLength = input.int(14, "Longueur KC", options=[10, 14, 20], group="-- Squeeze Momentum Indicator", tooltip="Définit la période des canaux Keltner pour le Squeeze.")
kcMult = input.float(1, "Multiplicateur KC", minval=0.9, maxval=1.2, step=0.1, group="-- Squeeze Momentum Indicator", tooltip="Ajuste l'écart des canaux Keltner en fonction de la volatilité.")
minMomentum = input.float(0.3, "Seuil min Momentum SMI (x avgRange)", minval=0.2, maxval=0.5, step=0.1, group="-- Squeeze Momentum Indicator", tooltip="Seuil minimum pour le momentum dans SMI, en multiples de la volatilité moyenne (avgRange). Par exemple, 0.5 exige que mom > 0.5*avgRange pour un signal haussier. Réduisez pour plus de signaux sur BTCUSD 15m.")

// Motifs de chandeliers japonais
useCandlePatternsInConditions = input.bool(false, "Activer motifs chandeliers", group="-- Motifs de Chandeliers Japonais")
pinBarBodyMaxPercent = input.float(25.0, "Pin Bar - % max du corps", minval=20, maxval=35, step=1.0, group="-- Motifs de Chandeliers Japonais", tooltip="Définit le pourcentage maximum du corps pour détecter un Pin Bar.") / 100.0
pinBarWickMinPercent = input.float(60.0, "Pin Bar - % min de la mèche", minval=50, maxval=70, step=1.0, group="-- Motifs de Chandeliers Japonais", tooltip="Définit le pourcentage minimum de la mèche pour détecter un Pin Bar.") / 100.0
marubozuWickMaxPercent = input.float(3.0, "Marubozu - % max des mèches", minval=2, maxval=5.0, step=0.5, group="-- Motifs de Chandeliers Japonais", tooltip="Définit le pourcentage maximum des mèches pour détecter un Marubozu.") / 100.0
marubozuBodyMinPercent = input.float(98.0, "Marubozu - % min du corps", minval=95, maxval=100.0, step=1.0, group="-- Motifs de Chandeliers Japonais", tooltip="Définit le pourcentage minimum du corps pour détecter un Marubozu.") / 100.0

// Fair Value Gaps
useFVG = input.bool(false, "Activer FVG (Imbalances)", group="-- Fair Value Gaps")
fvgMaxAgeBars = input.int(20, "Âge maximum des FVG (barres)", minval=10, maxval=100, group="-- Fair Value Gaps", tooltip="Définit le nombre maximum de barres après lesquelles un FVG est supprimé. Plage réduite pour le scalping.")
fvgAutoThreshold = input.bool(false, "Seuil automatique pour la détection", group="-- Fair Value Gaps", tooltip="Active un seuil dynamique pour détecter les FVG.")
fvgMinGapSize = input.float(0.1, "Taille minimale de l'écart (si seuil manuel)", minval=0.015, maxval=0.5, step=0.1, group="-- Fair Value Gaps", tooltip="Définit la taille minimale des écarts FVG (en pourcentage de volatilité).")
fvgVolumeThreshold = input.float(1.2, "Seuil de volume pour confirmer un FVG", minval=1.0, maxval=3.0, step=0.1, group="-- Fair Value Gaps", tooltip="Définit le seuil de volume (relatif à la moyenne) pour valider un FVG.")
fvgRejectionProximity = input.float(0.5, "Seuil de proximité pour le rejet (%)", minval=0.5, maxval=2, step=0.1, group="-- Fair Value Gaps", tooltip="Définit la proximité pour considérer un FVG comme rejeté (en pourcentage).")
fvgLookbackCandles = input.int(2, "Nombre de bougies pour détecter un FVG", minval=1, maxval=3, step=1, group="-- Fair Value Gaps", tooltip="Définit le nombre de bougies précédentes à analyser pour détecter un FVG.")
fvgExtendBars = input.int(1, "Extension des boîtes FVG (bougies)", minval=0, group="-- Fair Value Gaps", tooltip="Définit combien de bougies supplémentaires les boîtes FVG s'étendent à droite.")
fvgDetectConsecutive = input.bool(true, "Détecter les imbalances consécutives", group="-- Fair Value Gaps", tooltip="Active la détection des imbalances entre bougies adjacentes.")
fvgDetectLiquidity = input.bool(true, "Détecter les imbalances de liquidité", group="-- Fair Value Gaps", tooltip="Active la détection des imbalances liées à des balayages de liquidité.")
fvgLiquidityVolumeThreshold = input.float(1.5, "Seuil de volume pour imbalances de liquidité", minval=1.0, maxval=3.0, step=0.1, group="-- Fair Value Gaps", tooltip="Seuil de volume (relatif à la moyenne) pour confirmer un balayage de liquidité.")
fvgLiquidityMinMove = input.float(0.2, "Mouvement min après cassure (%)", minval=0.01, maxval=1.0, step=0.01, group="-- Fair Value Gaps", tooltip="Mouvement minimal après cassure d'un niveau S/R pour confirmer une imbalance de liquidité.")

// Fibonacci Retracement
useFibonacci = input.bool(false, "Activer Fibonacci", group="-- Fibonacci Retracement")
fibTimelapsHours = input.int(2, "Timelaps du retracement (heures)", minval=1, maxval=4, step=1, group="-- Fibonacci Retracement", tooltip="Définit la période (en heures) pour calculer les extrêmes Fibonacci. Plage réduite pour le scalping.")
fibTolerance = input.float(1.5, "Tolérance pour détecter les retracements", minval=0.1, maxval=5.0, step=0.1, group="-- Fibonacci Retracement", tooltip="Définit la tolérance (en %) pour détecter un prix proche d'un niveau Fibonacci.")
fibLevel1 = input.float(1, "Niveau Fibonacci 1", minval=0, maxval=1, step=0.001, group="-- Fibonacci Retracement", tooltip="Définit le premier niveau de retracement Fibonacci (100%).")
fibLevel2 = input.float(0.786, "Niveau Fibonacci 2", minval=0, maxval=1, step=0.001, group="-- Fibonacci Retracement", tooltip="Définit le deuxième niveau de retracement Fibonacci (78.6%).")
fibLevel3 = input.float(0.618, "Niveau Fibonacci 3", minval=0, maxval=1, step=0.001, group="-- Fibonacci Retracement", tooltip="Définit le troisième niveau de retracement Fibonacci (61.8%).")
fibLevel4 = input.float(0.5, "Niveau Fibonacci 4", minval=0, maxval=1, step=0.001, group="-- Fibonacci Retracement", tooltip="Définit le quatrième niveau de retracement Fibonacci (50%).")
fibLevel5 = input.float(0, "Niveau Fibonacci 5", minval=0, maxval=1, step=0.001, group="-- Fibonacci Retracement", tooltip="Définit le cinquième niveau de retracement Fibonacci (0%).")

// Support/Résistance
srGroup = "-- Support/Résistance Simplifié"
useSR = input.bool(false, "Activer les S/R", group=srGroup)
proximity = input.float(0.2, "Tolérance pour les conditions S/R (%)", minval=0.1, maxval=2, step=0.1, group=srGroup, tooltip="Tolérance (en %) pour les conditions de trading S/R.")
minSRTests = input.int(1, "Tests min pour S/R", minval=1, maxval=5, step=1, group=srGroup, tooltip="Nombre minimum de tests d’un niveau de support/résistance pour valider un signal. Par exemple, 2 exige au moins 2 touches du niveau. Augmentez pour des niveaux plus fiables, mais moins fréquents.")
useBreakout = input.bool(false, "Activer la détection de breakout", group=srGroup, tooltip="Active la détection des breakouts au-dessus de la résistance ou en dessous du support pour confirmer les signaux de trading.")
breakoutLookback = input.int(12, "Période de recherche pour breakout", minval=10, maxval=15, step=1, group=srGroup, tooltip="Nombre de barres à examiner pour identifier les niveaux de support/résistance à casser.")
breakoutVolumeThreshold = input.float(0.7, "Seuil de volume pour breakout", minval=0.5, maxval=1.2, step=0.1, group=srGroup, tooltip="Seuil minimal de volume (en multiples de la moyenne) pour valider un breakout.")
useRetest = input.bool(false, "Activer la confirmation par retest", group=srGroup, tooltip="Exige un retest du niveau de support ou de résistance après un breakout ou un rebond pour valider le signal.")
retestLookback = input.int(5, "Période de recherche pour retest", minval=5, maxval=6, step=1, group=srGroup, tooltip="Nombre de barres après un breakout ou un rebond pour détecter un retest.")
retestProximity = input.float(0.1, "Tolérance pour détecter un retest (%)", minval=0.05, maxval=0.5, step=0.05, group=srGroup, tooltip="Tolérance (en %) pour considérer que le prix a retesté un niveau.")

// Order Blocks
obGroup = "-- Order Blocks (OB)"
useOB = input.bool(false, "Activer les OB", group=obGroup)
obHorizontalSize = input.int(100, "Taille Horizontale des OB (barres)", minval=10, maxval=500, step=5, group=obGroup, tooltip="Définit la longueur horizontale des rectangles OB en nombre de barres.")
obMaxAgeBars = input.int(100, "Âge maximum des OB (barres)", minval=10, maxval=100, step=5, group=obGroup, tooltip="Définit le nombre maximum de barres après lesquelles un OB est supprimé. Plage réduite pour le scalping.")
threshold = input.float(1.5, title="Multiplicateur de volatilité pour un mouvement fort", minval=1, maxval=2, step=0.1, group=obGroup, tooltip="Définit l'ampleur minimale d'un mouvement fort (en multiples de la volatilité moyenne).")
definitionMove = input.int(2, title="Nombre de bougies pour détecter un mouvement fort", minval=2, maxval=3, step=1, group=obGroup, tooltip="Définit le nombre de bougies consécutives pour identifier un mouvement fort.")
minBarsBetweenOBs = input.float(0.15, title="Espacement minimum entre OB (barres)", minval=0.15, maxval=0.2, step=0.01, group=obGroup, tooltip="Définit le nombre minimum de barres entre deux OB pour éviter l'encombrement.")

atr_value = ta.atr(14)

// Fonctions utilitaires
calculateStopLoss(isLong, price) =>
    float pipsDistance = 0.0
    if pair == "BTCUSD"
        pipsDistance := 400.0
    else if pair == "XAUUSD"
        pipsDistance := 35.0
    else if pair == "CHFJPY"
        pipsDistance := 30.0
    else if pair == "GBPJPY" or pair == "EURJPY" or pair == "USDJPY"
        pipsDistance := 20.0
    else if pair == "EURUSD" or pair == "GBPUSD" or pair == "USDCAD" or pair == "EURGBP"
        pipsDistance := 15.0
    
    float baseStopDistance = pipsDistance * valeur_pips
    float adjustedStopDistance = baseStopDistance * (1.0 / sl_ratio)
    float stopLevel = isLong ? price - adjustedStopDistance : price + adjustedStopDistance
    stopLevel := math.round(stopLevel, 4)
    adjustedStopDistance := math.round(adjustedStopDistance, 4)
    [stopLevel, adjustedStopDistance]

calculateTakeProfit(isLong, entryPrice, stopLoss) =>
    float pipsDistance = 0.0
    if pair == "BTCUSD"
        pipsDistance := 400.0
    else if pair == "XAUUSD"
        pipsDistance := 35.0
    else if pair == "CHFJPY"
        pipsDistance := 30.0
    else if pair == "GBPJPY" or pair == "EURJPY" or pair == "USDJPY"
        pipsDistance := 20.0
    else if pair == "EURUSD" or pair == "GBPUSD" or pair == "USDCAD" or pair == "EURGBP"
        pipsDistance := 15.0
    
    float tpDistance = pipsDistance * tp_ratio * valeur_pips
    local_tp = isLong ? entryPrice + tpDistance : entryPrice - tpDistance
    local_tp := math.round(local_tp, 4)
    local_tp

updateTrailingStop(isLong, entryPrice, currentPrice, trailingStop, trailDistance) =>
    profitPct = isLong ? (currentPrice - entryPrice) / entryPrice * 100 : (entryPrice - currentPrice) / entryPrice * 100
    localTrailingActive = profitPct >= trailActivationPct
    newTrailingStop = trailingStop
    if localTrailingActive
        if isLong
            potentialTrail = currentPrice - trailDistance
            newTrailingStop := math.max(potentialTrail, trailingStop)
        else
            potentialTrail = currentPrice + trailDistance
            newTrailingStop := math.min(potentialTrail, trailingStop)
    newTrailingStop

isTrailingStopHit(isLong, currentPrice, trailingStop) =>
    if isLong
        currentPrice <= trailingStop
    else
        currentPrice >= trailingStop

canAddPosition(isLong, entryPrice, currentPrice, positionCount) =>
    profitPct = isLong ? (currentPrice - entryPrice) / entryPrice * 100 : (entryPrice - currentPrice) / entryPrice * 100
    profitCondition = profitPct >= pyramidMinProfit
    positionLimitCondition = positionCount < maxAddPositions
    profitCondition and positionLimitCondition

calculatePyramidSize(initialSize, positionCount) =>
    sizeReductionFactor = math.pow(pyramidSizeReducer, positionCount)
    newSize = initialSize * sizeReductionFactor
    newSize := math.ceil(newSize * 100) / 100
    math.max(newSize, 0.01)

calculateSqueeze() =>
    local_basis = ta.sma(close, bbLength)
    local_dev = bbMult * ta.stdev(close, bbLength)
    local_upperBB = local_basis + local_dev
    local_lowerBB = local_basis - local_dev
    local_ma = ta.sma(close, kcLength)
    local_trueRange = ta.tr(true)
    local_rangema = ta.sma(local_trueRange, kcLength)
    local_upperKC = local_ma + local_rangema * kcMult
    local_lowerKC = local_ma - local_rangema * kcMult
    local_squeezeOn = local_lowerBB > local_lowerKC and local_upperBB < local_upperKC
    local_squeezeOff = local_lowerBB < local_lowerKC or local_upperBB > local_upperKC
    local_momentum = ta.mom(close, 8)
    [local_squeezeOn, local_squeezeOff, local_momentum, local_upperBB, local_basis, local_lowerBB, local_upperKC, local_ma, local_lowerKC]

float adjustedTradeFees = enableBacktest ? backtestSpreadAdjustment : trade_fees

updateCapital() =>
    float updatedCapital = currentCapital
    float lastTradeProfit = 0.0
    float tradeFees = 0.0
    if strategy.closedtrades > 0
        lastTradeProfit := strategy.closedtrades.profit(strategy.closedtrades - 1)
        float lastTradeSize = strategy.closedtrades.size(strategy.closedtrades - 1)
        tradeFees := lastTradeSize * (adjustedTradeFees / 100.0)
        updatedCapital := updatedCapital + lastTradeProfit - tradeFees
    [updatedCapital, lastTradeProfit, tradeFees]

// CALCUL DES INDICATEURS TECHNIQUES
var bool isfirstBar = false
isConfirmed := barstate.isconfirmed
var bool isFirstBar = false
isFirstBar := barstate.isfirst

if pair == "BTCUSD"
    valeur_pips := 1.0
else if pair == "XAUUSD"
    valeur_pips := 10.0
else if pair == "USDCAD"
    valeur_pips := 10.0
else if pair == "USDJPY"
    valeur_pips := 9.13
else if pair == "GBPUSD"
    valeur_pips := 10.0
else if pair == "EURUSD"
    valeur_pips := 10.0
else if pair == "CHFJPY"
    valeur_pips := 7.7
else if pair == "GBPJPY"
    valeur_pips := 9.13
else if pair == "EURJPY"
    valeur_pips := 9.13
else if pair == "EURGBP"
    valeur_pips := 12.5

if isFirstBar
    initialUseSTMA := useSTMA
    initialUseRSI := useRSI
    initialUseStoch := useStoch
    initialUseADX := useADX
    initialUseMACD := useMACD
    initialUseOBV := useOBV
    initialUseSqueeze := useSqueeze
    initialUseCandlePatternsInConditions := useCandlePatternsInConditions
    initialUseFVG := useFVG
    initialUseFibonacci := useFibonacci
    initialUseSR := useSR
    initialUseOB := useOB
    initialUseDivergence := useDivergence
    initialUseBreakout := useBreakout
    backtestTradeCount := 0
    backtestTotalTradeDuration := 0.0
    backtestAverageTradeDuration := 0.0
    backtestProfitLoss := 0.0
    winningTrades := 0
    losingTrades := 0
    totalGrossProfit := 0.0
    totalGrossLoss := 0.0
    maxDrawdown := 0.0
    peakCapital := startingCapital
    currentEquity := startingCapital

var int adxLength = 7
var int adxSmoothing = 8
[diPlus, diMinus, adx] = ta.dmi(adxLength, adxSmoothing)
adxTrending = adx > adxThreshold
adxBullish = diPlus > diMinus
adxBearish = diMinus > diPlus
var float adxAvgRange = ta.sma(high - low, 14)

if useADX
    var int lookback = 50
    adxMax = ta.highest(adx, lookback)
    adxMin = ta.lowest(adx, lookback)
    priceMax = ta.highest(close, lookback)
    priceMin = ta.lowest(close, lookback)
    adxRange = adxMax - adxMin
    priceRange = priceMax - priceMin
    scaleFactor = adxRange != 0 ? priceRange / adxRange : 1.0
    adxNormalized := (adx - adxMin) * scaleFactor + priceMin
    diPlusMax = ta.highest(diPlus, lookback)
    diPlusMin = ta.lowest(diPlus, lookback)
    diMinusMax = ta.highest(diMinus, lookback)
    diMinusMin = ta.lowest(diMinus, lookback)
    diRange = math.max(diPlusMax - diPlusMin, diMinusMax - diMinusMin)
    scaleFactorDI = diRange != 0 ? priceRange / diRange : 1.0
    diPlusNormalized := (diPlus - diPlusMin) * scaleFactorDI + priceMin
    diMinusNormalized := (diMinus - diMinusMin) * scaleFactorDI + priceMin
else    
    adxNormalized := na
    diPlusNormalized := na
    diMinusNormalized := na

[macdLine, signalLine, histLine] = ta.macd(close, momentumPeriod, momentumPeriod * 2, momentumPeriod / 2)
macdCrossUp = ta.crossover(macdLine, signalLine)
macdCrossDown = ta.crossunder(macdLine, signalLine)

if useMACD
    var int lookback = 50
    macdMax = ta.highest(macdLine, lookback)
    macdMin = ta.lowest(macdLine, lookback)
    priceMax = ta.highest(close, lookback)
    priceMin = ta.lowest(close, lookback)
    macdRange = macdMax - macdMin
    priceRange = priceMax - priceMin
    scaleFactor = macdRange != 0 ? priceRange / macdRange : 1.0
    macdLineNormalized := (macdLine - macdMin) * scaleFactor + priceMin
    signalLineNormalized := (signalLine - macdMin) * scaleFactor + priceMin
else
    macdLineNormalized := na
    signalLineNormalized := na

var float obvMax = na
var float obvMin = na
var float priceMax = na
var float priceMin = na
var float obvRange = na
var float priceRange = na
var float scaleFactor = 1.0

obv = ta.obv
obvMA = ta.sma(obv, obvLength)
obvCrossUp = ta.crossover(obv, obvMA)
obvCrossDown = ta.crossunder(obv, obvMA)
obvRising = (obvCrossUp or (obv > obvMA and obv[1] > obvMA[1] and obv[2] > obvMA[2]))
obvFalling = (obvCrossDown or (obv < obvMA and obv[1] < obvMA[1] and obv[2] < obvMA[2]))

if useOBV
    var int lookback = 50
    obvMax := ta.highest(obv, lookback)
    obvMin := ta.lowest(obv, lookback)
    priceMax := ta.highest(close, lookback)
    priceMin := ta.lowest(close, lookback)
    obvRange := obvMax - obvMin
    priceRange := priceMax - priceMin
    scaleFactor := obvRange != 0 ? priceRange / obvRange : 1.0
    obvNormalized := (obv - obvMin) * scaleFactor + priceMin
    obvMANormalized := (obvMA - obvMin) * scaleFactor + priceMin
else
    obvNormalized := na
    obvMANormalized := na

stmaHMA := ta.wma(2 * ta.wma(close, stmaLength / 2) - ta.wma(close, stmaLength), math.round(math.sqrt(stmaLength)))
float stmaMA = stmaHMA
float stmaAtr = ta.atr(stmaPeriods)
float stmaUpTemp = stmaMA - stmaMultiplier * stmaAtr
float stmaUp1 = nz(stmaUp[1], stmaUpTemp)
stmaUp := close[1] > stmaUp1 ? math.max(stmaUpTemp, stmaUp1) : stmaUpTemp
float stmaDnTemp = stmaMA + stmaMultiplier * stmaAtr
float stmaDn1 = nz(stmaDn[1], stmaDnTemp)
stmaDn := close[1] < stmaDn1 ? math.min(stmaDnTemp, stmaDn1) : stmaDnTemp
stmaTrend := nz(stmaTrend[1], stmaTrend)
stmaTrend := stmaTrend == -1 and close > stmaDn1 ? 1 : stmaTrend == 1 and close < stmaUp1 ? -1 : stmaTrend
stmaUpPlot := stmaTrend == 1 ? stmaUp : na
stmaDnPlot := stmaTrend == 1 ? na : stmaDn
stmaMPlot := ohlc4

[squeezeOn_, squeezeOff_, mom_, upperBB_, basis_, lowerBB_, upperKC_, ma_, lowerKC_] = calculateSqueeze()
squeezeOn := squeezeOn_
squeezeOff := squeezeOff_
mom := mom_
upperBB := upperBB_
basis := basis_
lowerBB := lowerBB_
upperKC := upperKC_
ma := ma_
lowerKC := lowerKC_
squeezeOnStart := squeezeOn and not squeezeOn[1]
squeezeOffStart := squeezeOff and not squeezeOff[1]
candleVolatilitySma := ta.sma(high - low, 14)
bbWidthSmaPrecomputed := ta.sma(candleVolatilitySma, 14)
squeezeVolatilitySma := ta.sma(high - low, 14)

var float adxStdev = ta.stdev(adx, 14)
var float macdStdev = ta.stdev(macdLine, macdStdevLength)
var float obvStdev = ta.stdev(obv, obvLength)
var float bbWidthSma = bbWidthSmaPrecomputed

longTermSMA := ta.sma(close, longTermTrendLength)

currentATR := ta.atr(volatilityPeriod)
atrAverage := ta.sma(currentATR, 50)

// Calcul des indicateurs à chaque barre pour maintenir la cohérence
float tempRsiValue = ta.rsi(close, momentumPeriod)
float tempStochK = ta.stoch(close, high, low, momentumPeriod)
float tempStochD = ta.sma(tempStochK, stochSmoothD)
float tempStochKSmoothed = ta.sma(tempStochK, stochSmoothK)

// Mise à jour des valeurs mises en cache uniquement sur une nouvelle barre
if isNewBar
    cachedRsiValue := tempRsiValue
    cachedStochD := tempStochD
    cachedStochK := tempStochKSmoothed

float rsiValue = cachedRsiValue
float obvMAValue = useOBV ? ta.sma(obv, 14) : na  // Pas d'optimisation ici car obv change à chaque barre
stochK := cachedStochK
stochD := cachedStochD

var float rsi = na
if useRSI
    rsi := rsiValue
else
    rsi := na

bool momentumLongCondition = (useRSI and rsiValue > momentumThreshold) or (useStoch and stochK > momentumThreshold)
bool momentumShortCondition = (useRSI and rsiValue < momentumThreshold) or (useStoch and stochK < momentumThreshold)

color bgColor = na
if (not enableBacktest or showGraphicsInBacktest) and (useRSI or useStoch)
    bgColor := (useRSI and rsiValue > rsiOverbought) or momentumShortCondition ? color.new(color.purple, 70) :
               (useRSI and rsiValue < rsiOversold) or momentumLongCondition ? color.new(#9974c1, 70) : na
bgcolor(bgColor, title="Momentum Background")

bool sufficientVolume = true

bool bullishDivergence = false
bool bearishDivergence = false
if useDivergence and useRSI
    priceLow := ta.lowest(low, divergenceLookback)
    priceHigh := ta.highest(high, divergenceLookback)
    rsiLow := ta.lowest(rsiValue, divergenceLookback)
    rsiHigh := ta.highest(rsiValue, divergenceLookback)

    priceLowIndex = bar_index + ta.lowestbars(low, divergenceLookback)
    priceHighIndex = bar_index + ta.highestbars(high, divergenceLookback)
    rsiLowIndex = bar_index + ta.lowestbars(rsiValue, divergenceLookback)
    rsiHighIndex = bar_index + ta.highestbars(rsiValue, divergenceLookback)

    prevPriceLow = ta.lowest(low[divergenceLookback], divergenceLookback)
    prevPriceLowIndex = bar_index[divergenceLookback] + ta.lowestbars(low[divergenceLookback], divergenceLookback)
    prevRsiLow = ta.lowest(rsiValue[divergenceLookback], divergenceLookback)
    prevRsiLowIndex = bar_index[divergenceLookback] + ta.lowestbars(rsiValue[divergenceLookback], divergenceLookback)

    prevPriceHigh = ta.highest(high[divergenceLookback], divergenceLookback)
    prevPriceHighIndex = bar_index[divergenceLookback] + ta.highestbars(high[divergenceLookback], divergenceLookback)
    prevRsiHigh = ta.highest(rsiValue[divergenceLookback], divergenceLookback)
    prevRsiHighIndex = bar_index[divergenceLookback] + ta.highestbars(rsiValue[divergenceLookback], divergenceLookback)

    if priceLowIndex > prevPriceLowIndex and rsiLowIndex > prevRsiLowIndex
        bullishDivergence := priceLow < prevPriceLow and rsiLow > prevRsiLow

    if priceHighIndex > prevPriceHighIndex and rsiHighIndex > prevRsiHighIndex
        bearishDivergence := priceHigh > prevPriceHigh and rsiHigh < prevRsiHigh

bool bullishBreakout = false
bool bearishBreakout = false
var float breakoutSupport = na
var float breakoutResistance = na

if useBreakout
    breakoutSupport := supportLevel250
    breakoutResistance := resistanceLevel250
    bool resistanceCross = ta.crossover(close, breakoutResistance)
    bool supportCross = ta.crossunder(close, breakoutSupport)
    breakoutResistanceCross := not na(breakoutResistance) and resistanceCross
    breakoutSupportCross := not na(breakoutSupport) and supportCross
    if breakoutResistanceCross
        bullishBreakout := true
        lastBreakoutBar := bar_index
        lastBreakoutLevel := breakoutResistance
        lastBreakoutDirection := 1
    if breakoutSupportCross
        bearishBreakout := true
        lastBreakoutBar := bar_index
        lastBreakoutLevel := breakoutSupport
        lastBreakoutDirection := -1

bool bullishRebond = false
bool bearishRebond = false
if useSR
    if srLongCondition and not na(closestLevelPrice) and isSupport
        bullishRebond := true
        lastRebondBar := bar_index
        lastRebondLevel := closestLevelPrice
        lastRebondDirection := 1
    if srShortCondition and not na(closestLevelPrice) and not isSupport
        bearishRebond := true
        lastRebondBar := bar_index
        lastRebondLevel := closestLevelPrice
        lastRebondDirection := -1

bool retestConfirmedLong = true
bool retestConfirmedShort = true
if useRetest
    retestConfirmedLong := false
    retestConfirmedShort := false
    if lastBreakoutDirection == 1 and not na(lastBreakoutBar) and not na(lastBreakoutLevel)
        barsSinceBreakout = bar_index - lastBreakoutBar
        if barsSinceBreakout <= retestLookback
            retestTolerance = lastBreakoutLevel * (retestProximity / 100)
            if low <= lastBreakoutLevel + retestTolerance and low >= lastBreakoutLevel - retestTolerance and close > lastBreakoutLevel
                retestConfirmedLong := true
    if lastBreakoutDirection == -1 and not na(lastBreakoutBar) and not na(lastBreakoutLevel)
        barsSinceBreakout = bar_index - lastBreakoutBar
        if barsSinceBreakout <= retestLookback
            retestTolerance = lastBreakoutLevel * (retestProximity / 100)
            if high >= lastBreakoutLevel - retestTolerance and high <= lastBreakoutLevel + retestTolerance and close < lastBreakoutLevel
                retestConfirmedShort := true
    if lastRebondDirection == 1 and not na(lastRebondBar) and not na(lastRebondLevel)
        barsSinceRebond = bar_index - lastRebondBar
        if barsSinceRebond <= retestLookback
            retestTolerance = lastRebondLevel * (retestProximity / 100)
            if low <= lastRebondLevel + retestTolerance and low >= lastRebondLevel - retestTolerance and close > lastRebondLevel
                retestConfirmedLong := true
    if lastRebondDirection == -1 and not na(lastRebondBar) and not na(lastRebondLevel)
        barsSinceRebond = bar_index - lastRebondBar
        if barsSinceRebond <= retestLookback
            retestTolerance = lastRebondLevel * (retestProximity / 100)
            if high >= lastRebondLevel - retestTolerance and high <= lastRebondLevel + retestTolerance and close < lastRebondLevel
                retestConfirmedShort := true

doji = math.abs(close - open) <= (high - low) * 0.05
bullishEngulfing = close > open and close[1] < open[1] and close > open[1] and open < close[1]
bearishEngulfing = close < open and close[1] > open[1] and close < open[1] and open > close[1]
pinBar = math.abs(close - open) <= (high - low) * pinBarBodyMaxPercent and ((high - math.max(close, open)) >= (high - low) * pinBarWickMinPercent or (math.min(close, open) - low) >= (high - low) * pinBarWickMinPercent)
hammer = pinBar and close > open and (math.min(close, open) - low) >= (high - low) * pinBarWickMinPercent
invertedHammer = pinBar and close > open and (high - math.max(close, open)) >= (high - low) * pinBarWickMinPercent
pinBarBullish = pinBar and close > open and not hammer and not invertedHammer
shootingStar = pinBar and close < open and (high - math.max(close, open)) >= (high - low) * pinBarWickMinPercent
pinBarBearish = pinBar and close < open and not shootingStar
marubozuUp = math.abs(high - close) < (high - low) * marubozuWickMaxPercent and math.abs(open - low) < (high - low) * marubozuWickMaxPercent and math.abs(close - open) >= (high - low) * marubozuBodyMinPercent and close > open
marubozuDown = math.abs(high - open) < (high - low) * marubozuWickMaxPercent and math.abs(close - low) < (high - low) * marubozuWickMaxPercent and math.abs(close - open) >= (high - low) * marubozuBodyMinPercent and close < open

float tolerance = supportLevel * (proximity / 100)
if doji
    candlePattern := "Doji (Neutre)"
    candleIsUp := true
else if bullishEngulfing
    candlePattern := "Bullish Engulfing"
    candleIsUp := true
else if bearishEngulfing
    candlePattern := "Bearish Engulfing"
    candleIsUp := false
else if hammer and close[1] < close[2] and close >= supportLevel - tolerance
    candlePattern := "Hammer"
    candleIsUp := true
else if invertedHammer and close[1] < close[2] and close >= supportLevel - tolerance
    candlePattern := "Inverted Hammer"
    candleIsUp := true
else if pinBarBullish and close[1] < close[2] and close >= supportLevel - tolerance
    candlePattern := "Pin Bar Bullish"
    candleIsUp := true
else if shootingStar and close[1] > close[2] and close <= resistanceLevel + tolerance
    candlePattern := "Shooting Star"
    candleIsUp := false
else if pinBarBearish and close[1] > close[2] and close <= resistanceLevel + tolerance
    candlePattern := "Pin Bar Bearish"
    candleIsUp := false
else if marubozuUp
    candlePattern := "Marubozu Up"
    candleIsUp := true
else if marubozuDown
    candlePattern := "Marubozu Down"
    candleIsUp := false
else
    candlePattern := ""
    candleIsUp := false

if useCandlePatternsInConditions and candlePattern != "" and (not enableBacktest or showGraphicsInBacktest)
    string labelText = ""
    string tooltipText = ""
    color labelColor = na
    string labelStyle = candleIsUp ? label.style_label_up : label.style_label_down
    float labelPosition = candleIsUp ? low : high
    if candlePattern == "Doji (Neutre)"
        labelText := "Doji"
        tooltipText := "Neutre"
        labelColor := color.new(color.gray, 70)
    else if candlePattern == "Bullish Engulfing"
        labelText := "Bullish Engulfing"
        tooltipText := "Possible renversement"
        labelColor := color.new(color.green, 70)
    else if candlePattern == "Bearish Engulfing"
        labelText := "Bearish Engulfing"
        tooltipText := "Possible renversement"
        labelColor := color.new(color.red, 70)
    else if candlePattern == "Hammer"
        labelText := "Hammer"
        tooltipText := "Possible rebond"
        labelColor := color.new(color.green, 70)
    else if candlePattern == "Inverted Hammer"
        labelText := "Inverted Hammer"
        tooltipText := "Possible rebond"
        labelColor := color.new(color.green, 70)
    else if candlePattern == "Pin Bar Bullish"
        labelText := "Pin Bar Bullish"
        tooltipText := "Possible rebond"
        labelColor := color.new(color.green, 70)
    else if candlePattern == "Shooting Star"
        labelText := "Shooting Star"
        tooltipText := "Possible correction"
        labelColor := color.new(color.red, 70)
    else if candlePattern == "Pin Bar Bearish"
        labelText := "Pin Bar Bearish"
        tooltipText := "Possible correction"
        labelColor := color.new(color.red, 70)
    else if candlePattern == "Marubozu Up"
        labelText := "Marubozu Up"
        tooltipText := "Tendance forte"
        labelColor := color.new(color.green, 70)
    else if candlePattern == "Marubozu Down"
        labelText := "Marubozu Down"
        tooltipText := "Tendance forte"
        labelColor := color.new(color.red, 70)
    label.new(bar_index, labelPosition, labelText, color=labelColor, textcolor=color.white, style=labelStyle, size=size.normal, tooltip=tooltipText)

// Gestion des FVG (Imbalances)
lastClose = close[1]
lastOpen = open[1]
lastTime = time[1]
currentHigh = high
currentLow = low
currentTime = time
last2High = high[2]
last2Low = low[2]

barDeltaPercent = (lastClose - lastOpen) / (lastOpen * 100)
newTimeframe = timeframe.change("")
fvgVolumeCondition = volume[1] > avgVolume * fvgVolumeThreshold
thresholdFVG = fvgAutoThreshold ? ta.cum(math.abs(newTimeframe ? barDeltaPercent : 0)) / bar_index * 2 : fvgMinGapSize

bullishFVGBase = currentLow > last2High and lastClose > last2High and (fvgAutoThreshold ? barDeltaPercent > thresholdFVG : true)
bearishFVGBase = currentHigh < last2Low and lastClose < last2Low and (fvgAutoThreshold ? -barDeltaPercent > thresholdFVG : true)

fvgGapSizeBull = currentLow - last2High
fvgGapSizeBear = last2Low - currentHigh
fvgGapSizePercentBull = fvgGapSizeBull / fvgAvgRange * 100
fvgGapSizePercentBear = fvgGapSizeBear / fvgAvgRange * 100
minGapHeight = fvgAvgRange * 0.01

bullishFVG = bullishFVGBase and (fvgMinGapSize == 0.0 or fvgGapSizePercentBull >= fvgMinGapSize) and fvgGapSizeBull >= minGapHeight and fvgVolumeCondition
bearishFVG = bearishFVGBase and (fvgMinGapSize == 0.0 or fvgGapSizePercentBear >= fvgMinGapSize) and fvgGapSizeBear >= minGapHeight and fvgVolumeCondition

if fairValueGaps.size() > 0
    for i = 0 to fairValueGaps.size() - 1
        fvg = fairValueGaps.get(i)
        box.delete(fvg.topBox)
        box.delete(fvg.bottomBox)
    while fairValueGaps.size() > 0
        fairValueGaps.remove(0)

fvgRejected = false
fvgRejectedIndex = -1
var int lastRejectedBias = na
var float lastRejectedTop = na
var float lastRejectedBottom = na
var int activeFVGCount = 0

// Mise à jour de l'âge et gestion de l'affichage en une seule boucle
if not barstate.isfirst and fairValueGaps.size() > 0 and (not enableBacktest or showGraphicsInBacktest)
    for i = fairValueGaps.size() - 1 to 0 by 1
        var fairValueGap fvg = fairValueGaps.get(i)
        // Mise à jour de l'âge
        fvg.age := fvg.age + 1
        // Vérification de l'affichage
        shouldDisplay = (fvg.imbalanceType == "Consecutive" and fvgDetectConsecutive) or (fvg.imbalanceType == "Liquidity" and fvgDetectLiquidity) or (fvg.imbalanceType == "FVG")
        if shouldDisplay != fvg.isVisible
            fvg.isVisible := shouldDisplay
            if not shouldDisplay
                box.delete(fvg.topBox)
                box.delete(fvg.bottomBox)
        // Suppression des éléments obsolètes
        ageMultiplier = fvg.imbalanceType == "Consecutive" ? 0.5 : fvg.imbalanceType == "Liquidity" ? 1.5 : 1.0
        effectiveMaxAge = math.round(fvgMaxAgeBars * ageMultiplier)
        if fvg.age >= effectiveMaxAge
            if fvg.isVisible  // S'assurer que les boîtes sont supprimées uniquement si elles sont visibles
                box.delete(fvg.topBox)
                box.delete(fvg.bottomBox)
            fairValueGaps.remove(i)
        else
            fairValueGaps.set(i, fvg)

// Vérification des rejets avec limitation des itérations
if not barstate.isfirst and fairValueGaps.size() > 0
    activeFVGCount := 0
    for i = 0 to fairValueGaps.size() - 1
        var fairValueGap fvg = fairValueGaps.get(i)
        if not na(fvg.bias) and fvg.isVisible
            activeFVGCount := activeFVGCount + 1
            top = fvg.top
            bottom = fvg.bottom
            isBull = fvg.bias == 1
            fvgClose = close
            fvgHeight = top - bottom
            proximityThreshold = fvgHeight * (fvgRejectionProximity / 100.0)
            if isBull and fvgClose >= bottom and fvgClose <= top
                fvgRejected := true
                fvgRejectedIndex := i
                lastRejectedBias := fvg.bias
                lastRejectedTop := top
                lastRejectedBottom := bottom
                fvg.bias := na
                fairValueGaps.set(i, fvg)
                break  // Sortie anticipée après avoir trouvé un rejet
            else if not isBull and fvgClose <= top and fvgClose >= bottom
                fvgRejected := true
                fvgRejectedIndex := i
                lastRejectedBias := fvg.bias
                lastRejectedTop := top
                lastRejectedBottom := bottom
                fvg.bias := na
                fairValueGaps.set(i, fvg)
                break  // Sortie anticipée après avoir trouvé un rejet

var int lastFVGBar = na
var int lastFVGMainLabelBar = na
var int minBarsBetweenFVGs = 2

if useFVG and bullishFVG and (not enableBacktest or showGraphicsInBacktest)
    canDisplayFVG = na(lastFVGBar) or (bar_index - lastFVGBar) >= minBarsBetweenFVGs
    if canDisplayFVG
        topFVG := currentLow
        bottomFVG := last2High
        if topFVG < bottomFVG
            temp = topFVG
            topFVG := bottomFVG
            bottomFVG := temp
        height = topFVG - bottomFVG
        if height < fvgAvgRange * 0.01
            topFVG := topFVG + (fvgAvgRange * 0.01 - height) / 2
            bottomFVG := bottomFVG - (fvgAvgRange * 0.01 - height) / 2
        gapSizeBull = fvgGapSizeBull
        gapSizePercentBull = fvgGapSizePercentBull
        fvgSize = gapSizePercentBull
        fvgAge = 0
        volumeRatio = volume / avgVolume
        fvgColor = color.new(color.green, 50)
        var box topBox = na
        var box bottomBox = na
        topBox := box.new(left=bar_index - 1, right=bar_index + fvgExtendBars, top=topFVG, bottom=math.avg(topFVG, bottomFVG), border_color=fvgColor, bgcolor=fvgColor, xloc=xloc.bar_index)
        bottomBox := box.new(left=bar_index - 1, right=bar_index + fvgExtendBars, top=math.avg(topFVG, bottomFVG), bottom=bottomFVG, border_color=fvgColor, bgcolor=fvgColor, xloc=xloc.bar_index)
        fairValueGaps.push(fairValueGap.new(topFVG, bottomFVG, 1, topBox, bottomBox, bar_index, time, bar_index, "FVG", true, 0))
        lastFVGBar := bar_index
        lastBullishFVGDetected := true
        lastBullishFVGBar := bar_index
        lastBearishFVGDetected := false
        lastImbalanceType := "FVG"

if useFVG and bearishFVG and (not enableBacktest or showGraphicsInBacktest)
    canDisplayFVG = na(lastFVGBar) or (bar_index - lastFVGBar) >= minBarsBetweenFVGs
    if canDisplayFVG
        topFVG := last2Low
        bottomFVG := currentHigh
        if topFVG < bottomFVG
            temp = topFVG
            topFVG := bottomFVG
            bottomFVG := temp
        height = topFVG - bottomFVG
        if height < fvgAvgRange * 0.01
            topFVG := topFVG + (fvgAvgRange * 0.01 - height) / 2
            bottomFVG := bottomFVG - (fvgAvgRange * 0.01 - height) / 2
        gapSizeBear = fvgGapSizeBear
        gapSizePercentBear = fvgGapSizePercentBear
        fvgSize = gapSizePercentBear
        fvgAge = 0
        volumeRatio = volume / avgVolume
        fvgColor = color.new(color.red, 50)
        var box topBox = na
        var box bottomBox = na
        topBox := box.new(left=bar_index - 1, right=bar_index + fvgExtendBars, top=topFVG, bottom=math.avg(topFVG, bottomFVG), border_color=fvgColor, bgcolor=fvgColor, xloc=xloc.bar_index)
        bottomBox := box.new(left=bar_index - 1, right=bar_index + fvgExtendBars, top=math.avg(topFVG, bottomFVG), bottom=bottomFVG, border_color=fvgColor, bgcolor=fvgColor, xloc=xloc.bar_index)
        fairValueGaps.push(fairValueGap.new(topFVG, bottomFVG, -1, topBox, bottomBox, bar_index, time, bar_index, "FVG", true, 0))
        lastFVGBar := bar_index
        lastBearishFVGDetected := true
        lastBearishFVGBar := bar_index
        lastBullishFVGDetected := false
        lastImbalanceType := "FVG"

bool consecutiveBullishImbalance = false
bool consecutiveBearishImbalance = false
string consecutiveBullishStatus = "Non détectée"
string consecutiveBearishStatus = "Non détectée"
if fvgDetectConsecutive
    consecutiveBullishImbalance := low > high[1] and (fvgMinGapSize == 0.0 or (low - high[1]) / fvgAvgRange * 100 >= fvgMinGapSize) and volume > avgVolume * fvgVolumeThreshold
    consecutiveBearishImbalance := high < low[1] and (fvgMinGapSize == 0.0 or (low[1] - high) / fvgAvgRange * 100 >= fvgMinGapSize) and volume > avgVolume * fvgVolumeThreshold
    consecutiveBullishStatus := consecutiveBullishImbalance ? "Détectée" : "Non détectée"
    consecutiveBearishStatus := consecutiveBearishImbalance ? "Détectée" : "Non détectée"

bool activeConsecutiveBullish = false
bool activeConsecutiveBearish = false
if not barstate.isfirst and fairValueGaps.size() > 0
    for i = 0 to fairValueGaps.size() - 1
        fvg = fairValueGaps.get(i)
        if not na(fvg.bias) and fvg.imbalanceType == "Consecutive"
            if fvg.bias == 1
                activeConsecutiveBullish := true
            else if fvg.bias == -1
                activeConsecutiveBearish := true

if useFVG and fvgDetectConsecutive and consecutiveBullishImbalance and (not enableBacktest or showGraphicsInBacktest)
    canDisplayImbalance = true
    if canDisplayImbalance
        topFVG := low
        bottomFVG := high[1]
        if topFVG < bottomFVG
            temp = topFVG
            topFVG := bottomFVG
            bottomFVG := temp
        height = topFVG - bottomFVG
        if height < fvgAvgRange * 0.01
            topFVG := topFVG + (fvgAvgRange * 0.01 - height) / 2
            bottomFVG := bottomFVG - (fvgAvgRange * 0.01 - height) / 2
        imbalanceColor = color.new(color.blue, 0)
        var box topBox = na
        var box bottomBox = na
        topBox := box.new(left=bar_index - 1, right=bar_index + 5, top=topFVG + (height * 0.05), bottom=topFVG - (height * 0.05), border_color=imbalanceColor, bgcolor=imbalanceColor, xloc=xloc.bar_index)
        bottomBox := box.new(left=bar_index - 1, right=bar_index + 5, top=bottomFVG + (height * 0.05), bottom=bottomFVG - (height * 0.05), border_color=imbalanceColor, bgcolor=imbalanceColor, xloc=xloc.bar_index)
        fairValueGaps.push(fairValueGap.new(topFVG, bottomFVG, 1, topBox, bottomBox, bar_index, time, bar_index, "Consecutive", true, 0))
        lastConsecutiveImbalanceBar := bar_index
        lastBullishFVGDetected := true
        lastBullishFVGBar := bar_index
        lastBearishFVGDetected := false
        lastImbalanceType := "Consecutive"

if useFVG and fvgDetectConsecutive and consecutiveBearishImbalance and (not enableBacktest or showGraphicsInBacktest)
    canDisplayImbalance = true
    if canDisplayImbalance
        topFVG := low[1]
        bottomFVG := high
        if topFVG < bottomFVG
            temp = topFVG
            topFVG := bottomFVG
            bottomFVG := temp
        height = topFVG - bottomFVG
        if height < fvgAvgRange * 0.01
            topFVG := topFVG + (fvgAvgRange * 0.01 - height) / 2
            bottomFVG := bottomFVG - (fvgAvgRange * 0.01 - height) / 2
        imbalanceColor = color.new(color.purple, 0)
        var box topBox = na
        var box bottomBox = na
        topBox := box.new(left=bar_index - 1, right=bar_index + 5, top=topFVG + (height * 0.05), bottom=topFVG - (height * 0.05), border_color=imbalanceColor, bgcolor=imbalanceColor, xloc=xloc.bar_index)
        bottomBox := box.new(left=bar_index - 1, right=bar_index + 5, top=bottomFVG + (height * 0.05), bottom=bottomFVG - (height * 0.05), border_color=imbalanceColor, bgcolor=imbalanceColor, xloc=xloc.bar_index)
        fairValueGaps.push(fairValueGap.new(topFVG, bottomFVG, -1, topBox, bottomBox, bar_index, time, bar_index, "Consecutive", true, 0))
        lastConsecutiveImbalanceBar := bar_index
        lastBearishFVGDetected := true
        lastBearishFVGBar := bar_index
        lastBullishFVGDetected := false
        lastImbalanceType := "Consecutive"

bool liquidityBullishImbalance = false
bool liquidityBearishImbalance = false
string liquidityBullishStatus = "Non détectée"
string liquidityBearishStatus = "Non détectée"
float volumeRatio = volume / avgVolume
float moveAfterBreakoutBull = 0.0
float moveAfterBreakoutBear = 0.0
if fvgDetectLiquidity
    float pivotHigh = ta.pivothigh(high, 5, 5)
    float pivotLow = ta.pivotlow(low, 5, 5)
    breakoutResistance := not na(pivotHigh) ? pivotHigh : breakoutResistance
    breakoutSupport := not na(pivotLow) ? pivotLow : breakoutSupport
    float liquidityTolerance = fvgAvgRange * 0.001
    bool resistanceCross = close > breakoutResistance - liquidityTolerance
    bool supportCross = close < breakoutSupport + liquidityTolerance
    breakoutResistanceCross := not na(breakoutResistance) and resistanceCross
    breakoutSupportCross := not na(breakoutSupport) and supportCross
    if breakoutResistanceCross
        moveAfterBreakoutBull := (close - breakoutResistance) / breakoutResistance * 100
        if volume > avgVolume * fvgLiquidityVolumeThreshold
            if moveAfterBreakoutBull >= fvgLiquidityMinMove
                liquidityBullishImbalance := true
                liquidityBullishStatus := "Détectée"
            else
                liquidityBullishStatus := "Non détectée"
        else
            liquidityBullishStatus := "Non détectée"
    else
        liquidityBullishStatus := "Non détectée"
    if breakoutSupportCross
        moveAfterBreakoutBear := (breakoutSupport - close) / breakoutSupport * 100
        if volume > avgVolume * fvgLiquidityVolumeThreshold
            if moveAfterBreakoutBear >= fvgLiquidityMinMove
                liquidityBearishImbalance := true
                liquidityBearishStatus := "Détectée"
            else
                liquidityBearishStatus := "Non détectée"
        else
            liquidityBearishStatus := "Non détectée"
    else
        liquidityBearishStatus := "Non détectée"

if useFVG and fvgDetectLiquidity and liquidityBullishImbalance and (not enableBacktest or showGraphicsInBacktest)
    canDisplayImbalance = true
    if canDisplayImbalance
        topFVG := close
        bottomFVG := breakoutResistance
        if topFVG < bottomFVG
            temp = topFVG
            topFVG := bottomFVG
            bottomFVG := temp
        height = topFVG - bottomFVG
        if height < fvgAvgRange * 0.01
            topFVG := topFVG + (fvgAvgRange * 0.01 - height) / 2
            bottomFVG := bottomFVG - (fvgAvgRange * 0.01 - height) / 2
        imbalanceColor = color.new(color.new(#006400, 50), 0)
        var box topBox = na
        var box bottomBox = na
        topBox := box.new(left=bar_index - 1, right=bar_index + 5, top=topFVG + (height * 0.05), bottom=topFVG - (height * 0.05), border_color=imbalanceColor, bgcolor=imbalanceColor, xloc=xloc.bar_index)
        bottomBox := box.new(left=bar_index - 1, right=bar_index + 5, top=bottomFVG + (height * 0.05), bottom=bottomFVG - (height * 0.05), border_color=imbalanceColor, bgcolor=imbalanceColor, xloc=xloc.bar_index)
        fairValueGaps.push(fairValueGap.new(topFVG, bottomFVG, 1, topBox, bottomBox, bar_index, time, bar_index, "Liquidity", true, 0))
        lastLiquidityImbalanceBar := bar_index
        lastBullishFVGDetected := true
        lastBullishFVGBar := bar_index
        lastBearishFVGDetected := false
        lastImbalanceType := "Liquidity"

if useFVG and fvgDetectLiquidity and liquidityBearishImbalance and (not enableBacktest or showGraphicsInBacktest)
    canDisplayImbalance = true
    if canDisplayImbalance
        topFVG := breakoutSupport
        bottomFVG := close
        if topFVG < bottomFVG
            temp = topFVG
            topFVG := bottomFVG
            bottomFVG := temp
        height = topFVG - bottomFVG
        if height < fvgAvgRange * 0.01
            topFVG := topFVG + (fvgAvgRange * 0.01 - height) / 2
            bottomFVG := bottomFVG - (fvgAvgRange * 0.01 - height) / 2
        imbalanceColor = color.new(color.new(#8B0000, 50), 0)
        var box topBox = na
        var box bottomBox = na
        topBox := box.new(left=bar_index - 1, right=bar_index + 5, top=topFVG + (height * 0.05), bottom=topFVG - (height * 0.05), border_color=imbalanceColor, bgcolor=imbalanceColor, xloc=xloc.bar_index)
        bottomBox := box.new(left=bar_index - 1, right=bar_index + 5, top=bottomFVG + (height * 0.05), bottom=bottomFVG - (height * 0.05), border_color=imbalanceColor, bgcolor=imbalanceColor, xloc=xloc.bar_index)
        fairValueGaps.push(fairValueGap.new(topFVG, bottomFVG, -1, topBox, bottomBox, bar_index, time, bar_index, "Liquidity", true, 0))
        lastLiquidityImbalanceBar := bar_index
        lastBearishFVGDetected := true
        lastBearishFVGBar := bar_index
        lastBullishFVGDetected := false
        lastImbalanceType := "Liquidity"

srSectionExecuted := true

// Ajout de la détection d'une nouvelle barre
isNewBar := ta.change(time("1")) != 0

// Calcul des niveaux de support/résistance à chaque barre pour maintenir la cohérence
float tempSupportLevel10 = ta.lowest(low, 10)
float tempResistanceLevel10 = ta.highest(high, 10)
int tempSupportBarIndex10 = bar_index + ta.lowestbars(low, 10)
int tempResistanceBarIndex10 = bar_index + ta.highestbars(high, 10)

float tempSupportLevel50 = ta.lowest(low, 50)
float tempResistanceLevel50 = ta.highest(high, 50)
int tempSupportBarIndex50 = bar_index + ta.lowestbars(low, 50)
int tempResistanceBarIndex50 = bar_index + ta.highestbars(high, 50)

float tempSupportLevel100 = ta.lowest(low, 100)
float tempResistanceLevel100 = ta.highest(high, 100)
int tempSupportBarIndex100 = bar_index + ta.lowestbars(low, 100)
int tempResistanceBarIndex100 = bar_index + ta.highestbars(high, 100)

float tempSupportLevel250 = ta.lowest(low, 250)
float tempResistanceLevel250 = ta.highest(high, 250)
int tempSupportBarIndex250 = bar_index + ta.lowestbars(low, 250)
int tempResistanceBarIndex250 = bar_index + ta.highestbars(high, 250)

float tempSupportLevel = ta.lowest(low, 10)
float tempResistanceLevel = ta.highest(high, 10)

// Mise à jour des valeurs mises en cache uniquement sur une nouvelle barre
if isNewBar
    cachedSupportLevel10 := tempSupportLevel10
    cachedResistanceLevel10 := tempResistanceLevel10
    supportBarIndex10 := tempSupportBarIndex10
    resistanceBarIndex10 := tempResistanceBarIndex10

    cachedSupportLevel50 := tempSupportLevel50
    cachedResistanceLevel50 := tempResistanceLevel50
    supportBarIndex50 := tempSupportBarIndex50
    resistanceBarIndex50 := tempResistanceBarIndex50

    cachedSupportLevel100 := tempSupportLevel100
    cachedResistanceLevel100 := tempResistanceLevel100
    supportBarIndex100 := tempSupportBarIndex100
    resistanceBarIndex100 := tempResistanceBarIndex100

    cachedSupportLevel250 := tempSupportLevel250
    cachedResistanceLevel250 := tempResistanceLevel250
    supportBarIndex250 := tempSupportBarIndex250
    resistanceBarIndex250 := tempResistanceBarIndex250

    cachedSupportLevel := tempSupportLevel
    cachedResistanceLevel := tempResistanceLevel

// Utilisation des valeurs mises en cache
supportLevel10 := cachedSupportLevel10
resistanceLevel10 := cachedResistanceLevel10
supportLevel50 := cachedSupportLevel50
resistanceLevel50 := cachedResistanceLevel50
supportLevel100 := cachedSupportLevel100
resistanceLevel100 := cachedResistanceLevel100
supportLevel250 := cachedSupportLevel250
resistanceLevel250 := cachedResistanceLevel250
supportLevel := cachedSupportLevel
resistanceLevel := cachedResistanceLevel

int futureBars = 1
rightBar = bar_index + futureBars

if useSR
    srLongCondition := true
    srShortCondition := true
    closestLevelPrice := na
    isSupport := false
    if not na(supportLevel)
        srTolerance = supportLevel * (proximity / 100)
        if close >= supportLevel - srTolerance and close <= supportLevel + srTolerance
            testCountSupport10 := testCountSupport10 + 1
            closestLevelPrice := supportLevel
            isSupport := true
    if not na(resistanceLevel)
        srTolerance = resistanceLevel * (proximity / 100)
        if close >= resistanceLevel - srTolerance and close <= resistanceLevel + srTolerance
            testCountResistance10 := testCountResistance10 + 1
            if na(closestLevelPrice)
                closestLevelPrice := resistanceLevel
                isSupport := false
    if not na(closestLevelPrice)
        if isSupport and close >= closestLevelPrice and testCountSupport10 >= minSRTests and close[1] < close and (momentumLongCondition or (not useRSI and not useStoch))
            srLongCondition := true
        else
            srLongCondition := false
        if not isSupport and close <= closestLevelPrice and testCountResistance10 >= minSRTests and close[1] > close and (momentumShortCondition or (not useRSI and not useStoch))
            srShortCondition := true
        else
            srShortCondition := false
    else
        srLongCondition := false
        srShortCondition := false
else
    srLongCondition := true
    srShortCondition := true

is_within_seven_days = time >= last7Days
strongDownMove = ta.lowest(close, definitionMove) == close and (open - close) > (threshold * avgRange)
strongUpMove = ta.highest(close, definitionMove) == close and (close - open) > (threshold * avgRange)
lastBullishBeforeDrop = strongDownMove and close[1] > open[1]
lastBearishBeforeUp = strongUpMove and close[1] < open[1]
topOBBullish = ta.valuewhen(lastBullishBeforeDrop, high[1], 0)
bottomOBBullish = ta.valuewhen(lastBullishBeforeDrop, low[1], 0)
leftBarBullish = ta.valuewhen(lastBullishBeforeDrop, bar_index[1], 0)
rightBarBullish = leftBarBullish + obHorizontalSize
customOBBullish = bottomOBBullish
moveAmplitudeBullish = ta.valuewhen(lastBullishBeforeDrop, open - close, 0)
volumeAtBullishOB = ta.valuewhen(lastBullishBeforeDrop, volume[1], 0)
volumeRelBullishOB = volumeAtBullishOB / avgVolume
adviceBullish = volumeRelBullishOB > 1.5 and moveAmplitudeBullish > (threshold * avgRange) ? "Forte pression vendeuse, surveillez cassure sous " + str.tostring(customOBBullish, "#.##") + "$" : "Mouvement faible, surveillez consolidation à " + str.tostring(topOBBullish, "#.##") + "$"
topOBBearish = ta.valuewhen(lastBearishBeforeUp, high[1], 0)
bottomOBBearish = ta.valuewhen(lastBearishBeforeUp, low[1], 0)
leftBarBearish = ta.valuewhen(lastBearishBeforeUp, bar_index[1], 0)
rightBarBearish = leftBarBearish + obHorizontalSize
customOBBearish = bottomOBBearish
moveAmplitudeBearish = ta.valuewhen(lastBearishBeforeUp, close - open, 0)
volumeAtBearishOB = ta.valuewhen(lastBearishBeforeUp, volume[1], 0)
volumeRelBearishOB = volumeAtBearishOB / avgVolume
adviceBearish = volumeRelBullishOB > 1.5 and moveAmplitudeBearish > (threshold * avgRange) ? "Forte pression acheteuse, surveillez cassure sur " + str.tostring(topOBBearish, "#.##") + "$" : "Mouvement faible, surveillez consolidation à " + str.tostring(customOBBearish, "#.##") + "$"

// Gestion des order blocks en une seule boucle
if orderBlocks.size() > 0 and (not enableBacktest or showGraphicsInBacktest)
    for i = orderBlocks.size() - 1 to 0 by 1
        var orderBlock ob = orderBlocks.get(i)
        ob.age := ob.age + 1
        if ob.age >= obMaxAgeBars
            box.delete(ob.box)
            orderBlocks.remove(i)
        else
            orderBlocks.set(i, ob)

if lastBullishBeforeDrop and useOB and (na(lastOBBar) or bar_index - lastOBBar >= minBarsBetweenOBs) and (not enableBacktest or showGraphicsInBacktest)    
    obBox = box.new(left=leftBarBullish, right=rightBarBullish, top=topOBBullish, bottom=bottomOBBullish, border_color=color.red, bgcolor=color.new(color.red, 80))
    orderBlocks.push(orderBlock.new(topOBBullish, bottomOBBullish, -1, obBox, bar_index, 0))
    lastOBBar := bar_index
    lastBullishOBDetected := true
    lastBullishOBBar := bar_index
    lastBearishOBDetected := false

if lastBearishBeforeUp and useOB and (na(lastOBBar) or bar_index - lastOBBar >= minBarsBetweenOBs) and (not enableBacktest or showGraphicsInBacktest)
    obBox = box.new(left=leftBarBearish, right=rightBarBearish, top=topOBBearish, bottom=bottomOBBearish, border_color=color.green, bgcolor=color.new(color.green, 80))
    orderBlocks.push(orderBlock.new(topOBBearish, bottomOBBearish, 1, obBox, bar_index, 0))
    lastOBBar := bar_index
    lastBearishOBDetected := true
    lastBearishOBBar := bar_index
    lastBullishOBDetected := false

fibTimelapsSeconds = fibTimelapsHours * 60 * 60
barsInPeriod = math.round(fibTimelapsSeconds / timeframeSeconds)
barsInPeriod := math.max(barsInPeriod, 1)
barsInPeriod := barsInPeriod * 2
trendAverage := ta.sma(close, barsInPeriod)
minTimeOffsetSeconds = 30 * 60
barsToExcludeInitial = math.round(minTimeOffsetSeconds / timeframeSeconds)
barsToExcludeInitial := math.max(barsToExcludeInitial, 0)

float highestHigh = na
float lowestLow = na

for i = 0 to barsInPeriod - 1
    if i < barsToExcludeInitial
        continue
    float candleHigh = high[i]
    float candleLow = low[i]
    if na(candleHigh) or na(candleLow)
        continue
    if na(highestHigh)
        highestHigh := candleHigh
    if na(lowestLow)
        lowestLow := candleLow
    highestHigh := math.max(highestHigh, candleHigh)
    lowestLow := math.min(lowestLow, candleLow)

if na(highestHigh) or na(lowestLow)
    highestHigh := high
    lowestLow := low

fibHigh := highestHigh
fibLow := lowestLow

if not na(fibHigh) and not na(fibLow)
    fibRange := fibHigh - fibLow
    if trendAverage > fibLow
        fibLevel1Price := fibHigh - fibRange * (1 - fibLevel1)
        fibLevel2Price := fibHigh - fibRange * (1 - fibLevel2)
        fibLevel3Price := fibHigh - fibRange * (1 - fibLevel3)
        fibLevel4Price := fibHigh - fibRange * (1 - fibLevel4)
        fibLevel5Price := fibHigh - fibRange * (1 - fibLevel5)
    else
        fibLevel1Price := fibLow + fibRange * (1 - fibLevel1)
        fibLevel2Price := fibLow + fibRange * (1 - fibLevel2)
        fibLevel3Price := fibLow + fibRange * (1 - fibLevel3)
        fibLevel4Price := fibLow + fibRange * (1 - fibLevel4)
        fibLevel5Price := fibLow + fibRange * (1 - fibLevel5)

float tolerancePrice = 0.0
if not na(fibRange)
    tolerancePrice := fibRange * (fibTolerance / 100)
    nearFibLevelBullish := (close >= fibLevel1Price - tolerancePrice and close <= fibLevel1Price + tolerancePrice) or
                           (close >= fibLevel2Price - tolerancePrice and close <= fibLevel2Price + tolerancePrice) or
                           (close >= fibLevel3Price - tolerancePrice and close <= fibLevel3Price + tolerancePrice) or
                           (close >= fibLevel4Price - tolerancePrice and close <= fibLevel4Price + tolerancePrice) or
                           (close >= fibLevel5Price - tolerancePrice and close <= fibLevel5Price + tolerancePrice)
    nearFibLevelBearish := nearFibLevelBullish

if useFibonacci and not na(fibRange)
    tolerancePrice := fibRange * (fibTolerance / 100)
    if close >= fibLevel1Price - tolerancePrice and close <= fibLevel1Price + tolerancePrice
        fibClosestLevel := float(fibLevel1)
        fibClosestLevelStr := str.tostring(fibLevel1 * 100, "#.##") + "%"
        fibClosestLevelValue := fibLevel1Price
    else if close >= fibLevel2Price - tolerancePrice and close <= fibLevel2Price + tolerancePrice
        fibClosestLevel := float(fibLevel2)
        fibClosestLevelStr := str.tostring(fibLevel2 * 100, "#.##") + "%"
        fibClosestLevelValue := fibLevel2Price
    else if close >= fibLevel3Price - tolerancePrice and close <= fibLevel3Price + tolerancePrice
        fibClosestLevel := float(fibLevel3)
        fibClosestLevelStr := str.tostring(fibLevel3 * 100, "#.##") + "%"
        fibClosestLevelValue := fibLevel3Price
    else if close >= fibLevel4Price - tolerancePrice and close <= fibLevel4Price + tolerancePrice
        fibClosestLevel := float(fibLevel4)
        fibClosestLevelStr := str.tostring(fibLevel4 * 100, "#.##") + "%"
        fibClosestLevelValue := fibLevel4Price
    else if close >= fibLevel5Price - tolerancePrice and close <= fibLevel5Price + tolerancePrice
        fibClosestLevel := float(fibLevel5)
        fibClosestLevelStr := str.tostring(fibLevel5 * 100, "#.##") + "%"
        fibClosestLevelValue := fibLevel5Price

if not na(fibClosestLevel)
    fibIsUp := true

if useFibonacci and not na(fibHigh) and not na(fibLow) and (not enableBacktest or showGraphicsInBacktest)
    futureBars := 1
    if not na(fibLine1)
        line.delete(fibLine1[1])
        label.delete(fibLabel1[1])
        line.delete(fibLine2[1])
        label.delete(fibLabel2[1])
        line.delete(fibLine3[1])
        label.delete(fibLabel3[1])
        line.delete(fibLine4[1])
        label.delete(fibLabel4[1])
        line.delete(fibLine5[1])
        label.delete(fibLabel5[1])
        box.delete(fibBoxBetween50and618[1])
        box.delete(fibBoxBetween618and786[1])
    fibLine1 := line.new(bar_index - srDisplayPeriodBars, fibLevel1Price, bar_index + futureBars, fibLevel1Price, color=color.new(color.white, 70), style=line.style_solid, xloc=xloc.bar_index)
    fibLabel1 := label.new(bar_index + futureBars, fibLevel1Price, str.tostring(fibLevel1, "#.###") + " | " + str.tostring(fibLevel1Price, "#.##") + " $", color=color.new(color.white, 70), textcolor=color.black, style=label.style_label_left)
    fibLine2 := line.new(bar_index - srDisplayPeriodBars, fibLevel2Price, bar_index + futureBars, fibLevel2Price, color=color.new(color.blue, 70), style=line.style_solid, xloc=xloc.bar_index)
    fibLabel2 := label.new(bar_index + futureBars, fibLevel2Price, str.tostring(fibLevel2, "#.###") + " | " + str.tostring(fibLevel2Price, "#.##") + " $", color=color.new(color.blue, 70), textcolor=color.white, style=label.style_label_left)
    fibLine3 := line.new(bar_index - srDisplayPeriodBars, fibLevel3Price, bar_index + futureBars, fibLevel3Price, color=color.new(color.blue, 70), style=line.style_solid, xloc=xloc.bar_index)
    fibLabel3 := label.new(bar_index + futureBars, fibLevel3Price, str.tostring(fibLevel3, "#.###") + " | " + str.tostring(fibLevel3Price, "#.##") + " $", color=color.new(color.blue, 70), textcolor=color.white, style=label.style_label_left)
    fibLine4 := line.new(bar_index - srDisplayPeriodBars, fibLevel4Price, bar_index + futureBars, fibLevel4Price, color=color.new(color.blue, 70), style=line.style_solid, xloc=xloc.bar_index)
    fibLabel4 := label.new(bar_index + futureBars, fibLevel4Price, str.tostring(fibLevel4, "#.###") + " | " + str.tostring(fibLevel4Price, "#.##") + " $", color=color.new(color.blue, 70), textcolor=color.white, style=label.style_label_left)
    fibLine5 := line.new(bar_index - srDisplayPeriodBars, fibLevel5Price, bar_index + futureBars, fibLevel5Price, color=color.new(color.white, 70), style=line.style_solid, xloc=xloc.bar_index)
    fibLabel5 := label.new(bar_index + futureBars, fibLevel5Price, str.tostring(fibLevel5, "#.###") + " | " + str.tostring(fibLevel5Price, "#.##") + " $", color=color.new(color.white, 70), textcolor=color.black, style=label.style_label_left)
    fibBoxBetween50and618 := box.new(left=bar_index - srDisplayPeriodBars, right=bar_index + futureBars, top=fibLevel3Price, bottom=fibLevel4Price, border_color=color.blue, bgcolor=color.new(color.blue, 70), xloc=xloc.bar_index)
    fibBoxBetween618and786 := box.new(left=bar_index - srDisplayPeriodBars, right=bar_index + futureBars, top=fibLevel2Price, bottom=fibLevel3Price, border_color=color.blue, bgcolor=color.new(color.blue, 80), xloc=xloc.bar_index)

if useSR and (not enableBacktest or showGraphicsInBacktest)
    if not na(supportLine10) or not na(resistanceLine10)
        line.delete(supportLine10[1])
        line.delete(resistanceLine10[1])
        label.delete(supportLabel10[1])
        label.delete(resistanceLabel10[1])
    if not na(supportLevel10) and not na(supportBarIndex10)
        supportLine10 := line.new(supportBarIndex10, supportLevel10, rightBar, supportLevel10, color=color.new(color.red, 0), style=line.style_solid, width=1, xloc=xloc.bar_index)
        supportLabel10 := label.new(rightBar, supportLevel10, "Support: " + str.tostring(supportLevel10, "#.##") + "$", color=color.new(color.red, 0), textcolor=color.white, style=label.style_label_left)
    if not na(resistanceLevel10) and not na(resistanceBarIndex10)
        resistanceLine10 := line.new(resistanceBarIndex10, resistanceLevel10, rightBar, resistanceLevel10, color=color.new(color.green, 0), style=line.style_solid, width=1, xloc=xloc.bar_index)
        resistanceLabel10 := label.new(rightBar, resistanceLevel10, "Résistance: " + str.tostring(resistanceLevel10, "#.##") + "$", color=color.new(color.green, 0), textcolor=color.white, style=label.style_label_left)

if useSR and (not enableBacktest or showGraphicsInBacktest)
    if not na(supportLine50) or not na(resistanceLine50)
        line.delete(supportLine50[1])
        line.delete(resistanceLine50[1])
        label.delete(supportLabel50[1])
        label.delete(resistanceLabel50[1])
    if not na(supportLevel50) and not na(supportBarIndex50)
        supportLine50 := line.new(supportBarIndex50, supportLevel50, rightBar, supportLevel50, color=color.new(color.red, 0), style=line.style_solid, width=1, xloc=xloc.bar_index)
        supportLabel50 := label.new(rightBar, supportLevel50, "Support: " + str.tostring(supportLevel50, "#.##") + "$", color=color.new(color.red, 0), textcolor=color.white, style=label.style_label_left)
    if not na(resistanceLevel50) and not na(resistanceBarIndex50)
        resistanceLine50 := line.new(resistanceBarIndex50, resistanceLevel50, rightBar, resistanceLevel50, color=color.new(color.green, 0), style=line.style_solid, width=1, xloc=xloc.bar_index)
        resistanceLabel50 := label.new(rightBar, resistanceLevel50, "Résistance: " + str.tostring(resistanceLevel50, "#.##") + "$", color=color.new(color.green, 0), textcolor=color.white, style=label.style_label_left)

if useSR and (not enableBacktest or showGraphicsInBacktest)
    if not na(supportLine100) or not na(resistanceLine100)
        line.delete(supportLine100[1])
        line.delete(resistanceLine100[1])
        label.delete(supportLabel100[1])
        label.delete(resistanceLabel100[1])
    if not na(supportLevel100) and not na(supportBarIndex100)
        supportLine100 := line.new(supportBarIndex100, supportLevel100, rightBar, supportLevel100, color=color.new(color.red, 0), style=line.style_solid, width=1, xloc=xloc.bar_index)
        supportLabel100 := label.new(rightBar, supportLevel100, "Support: " + str.tostring(supportLevel100, "#.##") + "$", color=color.new(color.red, 0), textcolor=color.white, style=label.style_label_left)
    if not na(resistanceLevel100) and not na(resistanceBarIndex100)
        resistanceLine100 := line.new(resistanceBarIndex100, resistanceLevel100, rightBar, resistanceLevel100, color=color.new(color.green, 0), style=line.style_solid, width=1, xloc=xloc.bar_index)
        resistanceLabel100 := label.new(rightBar, resistanceLevel100, "Résistance: " + str.tostring(resistanceLevel100, "#.##") + "$", color=color.new(color.green, 0), textcolor=color.white, style=label.style_label_left)

if useSR and (not enableBacktest or showGraphicsInBacktest)
    if not na(supportLine250) or not na(resistanceLine250)
        line.delete(supportLine250[1])
        line.delete(resistanceLine250[1])
        label.delete(supportLabel250[1])
        label.delete(resistanceLabel250[1])
    if not na(supportLevel250) and not na(supportBarIndex250)
        supportLine250 := line.new(supportBarIndex250, supportLevel250, rightBar, supportLevel250, color=color.new(color.red, 0), style=line.style_solid, width=1, xloc=xloc.bar_index)
        supportLabel250 := label.new(rightBar, supportLevel250, "Support: " + str.tostring(supportLevel250, "#.##") + "$", color=color.new(color.red, 0), textcolor=color.white, style=label.style_label_left)
    if not na(resistanceLevel250) and not na(resistanceBarIndex250)
        resistanceLine250 := line.new(resistanceBarIndex250, resistanceLevel250, rightBar, resistanceLevel250, color=color.new(color.green, 0), style=line.style_solid, width=1, xloc=xloc.bar_index)
        resistanceLabel250 := label.new(rightBar, resistanceLevel250, "Résistance: " + str.tostring(resistanceLevel250, "#.##") + "$", color=color.new(color.green, 0), textcolor=color.white, style=label.style_label_left)

if array.size(indicatorPropsArray) == 0
    array.push(indicatorPropsArray, IndicatorProps.new("SMA Long Terme", color.new(#FFD700, 70), #000000, "Tendance "))
    array.push(indicatorPropsArray, IndicatorProps.new("SuperTrend", color.new(#FFD700, 70), #000000, "Tendance "))
    array.push(indicatorPropsArray, IndicatorProps.new("RSI", color.new(#808080, 70), #FFFFFF, "RSI: "))
    array.push(indicatorPropsArray, IndicatorProps.new("Divergence", color.new(#00FFFF, 70), #FFFFFF, "Divergence: "))
    array.push(indicatorPropsArray, IndicatorProps.new("Stochastique", color.new(#FF00FF, 70), #FFFFFF, "Stoch: "))
    array.push(indicatorPropsArray, IndicatorProps.new("ADX", color.new(#808080, 70), #FFFFFF, "ADX: "))
    array.push(indicatorPropsArray, IndicatorProps.new("MACD", color.new(#FFA500, 70), #000000, "Croisement "))
    array.push(indicatorPropsArray, IndicatorProps.new("OBV", color.new(#FFFF00, 70), #000000, "Signal "))
    array.push(indicatorPropsArray, IndicatorProps.new("Squeeze", color.new(#00FF00, 70), #000000, "Squeeze "))
    array.push(indicatorPropsArray, IndicatorProps.new("Candle", color.new(#FF00FF, 70), #FFFFFF, "Motif "))
    array.push(indicatorPropsArray, IndicatorProps.new("FVG", color.new(#089981, 70), #FFFFFF, "Taille: "))
    array.push(indicatorPropsArray, IndicatorProps.new("Fibonacci", color.new(#800000, 70), #FFFFFF, "Fibonacci "))
    array.push(indicatorPropsArray, IndicatorProps.new("S/R", color.new(#089981, 70), #FFFFFF, "Support: "))
    array.push(indicatorPropsArray, IndicatorProps.new("Breakout", color.new(#FF4500, 70), #FFFFFF, "Breakout: "))
    array.push(indicatorPropsArray, IndicatorProps.new("OB", color.new(#F23645, 70), #FFFFFF, "Haut: "))

// CONDITIONS DE TRADING AVEC VOTE MAJORITAIRE

bool longTermTrendUp = close > longTermSMA
bool longTermTrendDown = close < longTermSMA
bool sufficientVolatility = currentATR > volatilityThreshold * atrAverage

adxLongCondition = useADX ? (adxBullish and adxTrending and adx[1] > adxThreshold and diPlus > diMinus + minDIGap * adxStdev) : true
adxShortCondition = useADX ? (adxBearish and adxTrending and adx[1] > adxThreshold and diMinus > diPlus + minDIGap * adxStdev) : true

macdLongCondition = useMACD ? (macdCrossUp and macdLine > 0 and math.abs(macdLine - signalLine) > minMACDGap * macdStdev) : true
macdShortCondition = useMACD ? (macdCrossDown and macdLine < 0 and math.abs(macdLine - signalLine) > minMACDGap * macdStdev) : true

obvLongCondition = useOBV ? (obvRising and obv > obvMA + minOBVGap * obvStdev) : true
obvShortCondition = useOBV ? (obvFalling and obv < obvMA - minOBVGap * obvStdev) : true

squeezeLongCondition = useSqueeze ? (squeezeOff and mom > minMomentum * avgRange and (squeezeVolatilitySma > bbWidthSma)) : true
squeezeShortCondition = useSqueeze ? (squeezeOff and mom < -minMomentum * avgRange and (squeezeVolatilitySma > bbWidthSma)) : true

candleLongCondition = useCandlePatternsInConditions ? (bullishEngulfing or hammer or invertedHammer or pinBarBullish or marubozuUp) and candleVolatilitySma > bbWidthSmaPrecomputed : true
candleShortCondition = useCandlePatternsInConditions ? (bearishEngulfing or shootingStar or pinBarBearish or marubozuDown) and candleVolatilitySma > bbWidthSmaPrecomputed : true

bool fvgLongConditionLarge = false
bool fvgShortConditionLarge = false
bool fvgLongConditionConsecutive = false
bool fvgShortConditionConsecutive = false
bool fvgLongConditionLiquidity = false
bool fvgShortConditionLiquidity = false

if not barstate.isfirst and useFVG and fairValueGaps.size() > 0
    for i = 0 to fairValueGaps.size() - 1
        fvg = fairValueGaps.get(i)
        if not na(fvg.bias)
            top = fvg.top
            bottom = fvg.bottom
            isBull = fvg.bias == 1
            if fvg.imbalanceType == "FVG"
                if isBull and close <= top and close >= bottom
                    fvgLongConditionLarge := true
                else if not isBull and close >= bottom and close <= top
                    fvgShortConditionLarge := true
            else if fvg.imbalanceType == "Consecutive"
                if isBull and close <= top and close >= bottom
                    fvgLongConditionConsecutive := true
                else if not isBull and close >= bottom and close <= top
                    fvgShortConditionConsecutive := true
            else if fvg.imbalanceType == "Liquidity"
                if isBull and close <= top and close >= bottom
                    fvgLongConditionLiquidity := true
                else if not isBull and close >= bottom and close <= top
                    fvgShortConditionLiquidity := true

fvgLongCondition = useFVG ? (fvgLongConditionLarge or fvgLongConditionConsecutive or fvgLongConditionLiquidity) : true
fvgShortCondition = useFVG ? (fvgShortConditionLarge or fvgShortConditionConsecutive or fvgShortConditionLiquidity) : true

fibLongCondition = useFibonacci ? (nearFibLevelBullish and (fibClosestLevel == float(fibLevel3) or fibClosestLevel == float(fibLevel4)) and close[1] < close[2] and (momentumLongCondition or (not useRSI and not useStoch))) : true
fibShortCondition = useFibonacci ? (nearFibLevelBearish and (fibClosestLevel == float(fibLevel3) or fibClosestLevel == float(fibLevel4)) and close[1] > close[2] and (momentumShortCondition or (not useRSI and not useStoch))) : true

if not na(supportLevel)
    srTolerance = supportLevel * (proximity / 100)
    if close >= supportLevel - srTolerance and close <= supportLevel + srTolerance
        testCountSupport10 := testCountSupport10 + 1
if not na(resistanceLevel)
    srTolerance = resistanceLevel * (proximity / 100)
    if close >= resistanceLevel - srTolerance and close <= resistanceLevel + srTolerance
        testCountResistance10 := testCountResistance10 + 1

obLongCondition = useOB ? (lastBearishBeforeUp and close > customOBBearish and close <= customOBBearish + 0.5 * avgRange and volumeAtBearishOB > 1.5 * avgVolume) : true
obShortCondition = useOB ? (lastBullishBeforeDrop and close < customOBBullish and close >= customOBBullish - 0.5 * avgRange and volumeAtBullishOB > 1.5 * avgVolume) : true

stmaLongCondition = useSTMA ? (stmaTrend == 1 and close > stmaUp) : true
stmaShortCondition = useSTMA ? (stmaTrend == -1 and close < stmaDn) : true

trendScore := 0.0
if useSTMA
    if stmaTrend == 1
        trendScore := trendScore + 1.0
    else if stmaTrend == -1
        trendScore := trendScore - 1.0

if useADX
    if adxLongCondition
        trendScore := trendScore + 1.0
    else if adxShortCondition
        trendScore := trendScore - 1.0

if useOBV
    if obvLongCondition
        trendScore := trendScore + 1.0
    else if obvShortCondition
        trendScore := trendScore - 1.0

if useMACD
    if macdLongCondition
        trendScore := trendScore + 1.0
    else if macdShortCondition
        trendScore := trendScore - 1.0

if useSqueeze
    if squeezeLongCondition
        trendScore := trendScore + 1.0
    else if squeezeShortCondition
        trendScore := trendScore - 1.0

if useCandlePatternsInConditions
    if candleLongCondition
        trendScore := trendScore + 1.0
    else if candleShortCondition
        trendScore := trendScore - 1.0

if useSR
    if srLongCondition
        trendScore := trendScore + 1.0
    else if srShortCondition
        trendScore := trendScore - 1.0

if useOB
    if obLongCondition
        trendScore := trendScore + 1.0
    else if obShortCondition
        trendScore := trendScore - 1.0

if useFVG
    if fvgLongConditionLarge
        trendScore := trendScore + 1.0
    if fvgShortConditionLarge
        trendScore := trendScore - 1.0
    if fvgLongConditionConsecutive
        trendScore := trendScore + 0.5
    if fvgShortConditionConsecutive
        trendScore := trendScore - 0.5
    if fvgLongConditionLiquidity
        trendScore := trendScore + 1.5
    if fvgShortConditionLiquidity
        trendScore := trendScore - 1.5

if useRSI
    if rsiValue < 30
        trendScore := trendScore + 1.0
    else if rsiValue > 70
        trendScore := trendScore - 1.0
    else if rsiValue > momentumThreshold
        trendScore := trendScore + 1.0
    else if rsiValue < momentumThreshold
        trendScore := trendScore - 1.0

if useStoch
    if stochK > momentumThreshold
        trendScore := trendScore + 1.0
    else if stochK < momentumThreshold
        trendScore := trendScore - 1.0

if useFibonacci
    if fibLongCondition
        trendScore := trendScore + 1.0
    else if fibShortCondition
        trendScore := trendScore - 1.0

if useDivergence and useRSI
    if bullishDivergence
        trendScore := trendScore + 1.0
    else if bearishDivergence
        trendScore := trendScore - 1.0

if useBreakout
    if bullishBreakout
        trendScore := trendScore + 1.0
    else if bearishBreakout
        trendScore := trendScore - 1.0

bool canGenerateLong = true
bool canGenerateShort = true
if useSignalFilter
    if not na(lastSignalBar)
        barsSinceLastSignal = bar_index - lastSignalBar
        if barsSinceLastSignal < signalCooldownBars
            if lastSignalDirection == 1
                canGenerateShort := false
            else if lastSignalDirection == -1
                canGenerateLong := false

long_condition = longTermTrendUp and sufficientVolatility and sufficientVolume and trendScore >= float(signalThreshold) and (momentumLongCondition or (not useRSI and not useStoch)) and (bullishDivergence or not useDivergence) and (bullishBreakout or not useBreakout) and canGenerateLong and (retestConfirmedLong or not useRetest)
short_condition = longTermTrendDown and sufficientVolatility and sufficientVolume and trendScore <= -float(signalThreshold) and (momentumShortCondition or (not useRSI and not useStoch)) and (bearishDivergence or not useDivergence) and (bearishBreakout or not useBreakout) and canGenerateShort and (retestConfirmedShort or not useRetest)

if long_condition
    lastSignalBar := bar_index
    lastSignalDirection := 1
else if short_condition
    lastSignalBar := bar_index
    lastSignalDirection := -1

if isFirstBar
    currentDay := dayofmonth(time)
    dailyPnL := 0.0
    dailyLossLimitReached := false
    dailyGainLimitReached := false

if dayofmonth(time) != currentDay
    currentDay := dayofmonth(time)
    dailyPnL := 0.0
    dailyLossLimitReached := false
    dailyGainLimitReached := false

if strategy.position_size != 0
    float entryPrice = strategy.position_avg_price
    float currentPrice = close
    float positionPnL = strategy.position_size > 0 ? (currentPrice - entryPrice) / entryPrice * 100 : (entryPrice - currentPrice) / entryPrice * 100
    dailyPnL := positionPnL
else if strategy.closedtrades > 0
    float closedPnL = strategy.netprofit / strategy.initial_capital * 100
    dailyPnL := closedPnL

if (not enableBacktest or applyDailyLimitsInBacktest)
    if useMaxDailyLoss and dailyPnL <= -maxDailyLossPercent
        dailyLossLimitReached := true
    if useMaxDailyGain and dailyPnL >= maxDailyGainPercent
        dailyGainLimitReached := true
else
    dailyLossLimitReached := false
    dailyGainLimitReached := false

bool isWithinTradingHours = enableBacktest ? true : hour >= start_hour and hour <= end_hour
bool canTrade = isWithinTradingHours and not dailyLossLimitReached and not dailyGainLimitReached

if isConfirmed
    [newCapital, lastTradeProfit, tradeFees] = updateCapital()
    currentCapital := newCapital
    riskAmount := risque_par_trade
    if enableBacktest and strategy.closedtrades > 0
        backtestProfitLoss := backtestProfitLoss + lastTradeProfit - tradeFees
        currentEquity := currentEquity + lastTradeProfit - tradeFees
        peakCapital := math.max(peakCapital, currentEquity)
        drawdown = peakCapital - currentEquity
        maxDrawdown := math.max(maxDrawdown, drawdown)
        if lastTradeProfit > 0
            winningTrades := winningTrades + 1
            totalGrossProfit := totalGrossProfit + lastTradeProfit
        else
            losingTrades := losingTrades + 1
            totalGrossLoss := totalGrossLoss + math.abs(lastTradeProfit)

if long_condition and not in_trade and canTrade
    in_trade := true
    entry_price := close
    entry_capital := currentCapital
    float pipsDistance = 0.0
    if pair == "BTCUSD"
        pipsDistance := 400.0
    else if pair == "XAUUSD"
        pipsDistance := 35.0
    else if pair == "CHFJPY"
        pipsDistance := 30.0
    else if pair == "GBPJPY" or pair == "EURJPY" or pair == "USDJPY"
        pipsDistance := 20.0
    else if pair == "EURUSD" or pair == "GBPUSD" or pair == "USDCAD" or pair == "EURGBP"
        pipsDistance := 15.0
    float numPips = pipsDistance / sl_ratio
    lot_size := (currentCapital * (riskAmount / 100)) / (numPips * valeur_pips)
    lot_size := math.ceil(lot_size * 100) / 100
    [sl_level_temp, stopDistance_temp] = calculateStopLoss(true, entry_price)
    sl_level := sl_level_temp
    stopDistance := stopDistance_temp
    tp_level := calculateTakeProfit(true, entry_price, sl_level)
    trailingStop := sl_level
    trailingActive := false
    pyramidPositionCount := 0
    entry_time := time
    tradeStartBar := bar_index
    tradeEntryPrice := entry_price
    tradeTP := tp_level
    tradeSL := sl_level
    strategy.entry("Long", strategy.long, qty=lot_size)
    if enableBacktest
        backtestTradeCount := backtestTradeCount + 1

if short_condition and not in_trade and canTrade
    in_trade := true
    entry_price := close
    entry_capital := currentCapital
    float pipsDistance = 0.0
    if pair == "BTCUSD"
        pipsDistance := 400.0
    else if pair == "XAUUSD"
        pipsDistance := 35.0
    else if pair == "CHFJPY"
        pipsDistance := 30.0
    else if pair == "GBPJPY" or pair == "EURJPY" or pair == "USDJPY"
        pipsDistance := 20.0
    else if pair == "EURUSD" or pair == "GBPUSD" or pair == "USDCAD" or pair == "EURGBP"
        pipsDistance := 15.0
    float numPips = pipsDistance / sl_ratio
    lot_size := (currentCapital * (riskAmount / 100)) / (numPips * valeur_pips)
    lot_size := math.ceil(lot_size * 100) / 100
    [sl_level_temp, stopDistance_temp] = calculateStopLoss(false, entry_price)
    sl_level := sl_level_temp
    stopDistance := stopDistance_temp
    tp_level := calculateTakeProfit(false, entry_price, sl_level)
    trailingStop := sl_level
    trailingActive := false
    pyramidPositionCount := 0
    entry_time := time
    tradeStartBar := bar_index
    tradeEntryPrice := entry_price
    tradeTP := tp_level
    tradeSL := sl_level
    strategy.entry("Short", strategy.short, qty=lot_size)
    if enableBacktest
        backtestTradeCount := backtestTradeCount + 1

var bool wasInTrade = false
if in_trade and not wasInTrade
    if initialUseSTMA
        useSTMA := false
    if initialUseRSI
        useRSI := false
    if initialUseStoch
        useStoch := false
    if initialUseADX
        useADX := false
    if initialUseMACD
        useMACD := false
    if initialUseOBV
        useOBV := false
    if initialUseSqueeze
        useSqueeze := false
    if initialUseCandlePatternsInConditions
        useCandlePatternsInConditions := false
    if initialUseFVG
        useFVG := false
    if initialUseFibonacci
        useFibonacci := false
    if initialUseSR
        useSR := false
    if initialUseOB
        useOB := false
    if initialUseDivergence
        useDivergence := false
    if initialUseBreakout
        useBreakout := false

if not in_trade and wasInTrade
    if initialUseSTMA
        useSTMA := initialUseSTMA
    if initialUseRSI
        useRSI := initialUseRSI
    if initialUseStoch
        useStoch := initialUseStoch
    if initialUseADX
        useADX := initialUseADX
    if initialUseMACD
        useMACD := initialUseMACD
    if initialUseOBV
        useOBV := initialUseOBV
    if initialUseSqueeze
        useSqueeze := initialUseSqueeze
    if initialUseCandlePatternsInConditions
        useCandlePatternsInConditions := initialUseCandlePatternsInConditions
    if initialUseFVG
        useFVG := initialUseFVG
    if initialUseFibonacci
        useFibonacci := initialUseFibonacci
    if initialUseSR
        useSR := initialUseSR
    if initialUseOB
        useOB := initialUseOB
    if initialUseDivergence
        useDivergence := initialUseDivergence
    if initialUseBreakout
        useBreakout := initialUseBreakout

wasInTrade := in_trade

if not barstate.isfirst
    if in_trade and not wasInTrade and (not enableBacktest or showGraphicsInBacktest)
        if not na(supportLine10)
            line.delete(supportLine10[1])
            label.delete(supportLabel10[1])
            supportLine10 := na
            supportLabel10 := na
        if not na(resistanceLine10)
            line.delete(resistanceLine10[1])
            label.delete(resistanceLabel10[1])
            resistanceLine10 := na
            resistanceLabel10 := na
        if not na(supportLine50)
            line.delete(supportLine50[1])
            label.delete(supportLabel50[1])
            supportLine50 := na
            supportLabel50 := na
        if not na(resistanceLine50)
            line.delete(resistanceLine50[1])
            label.delete(resistanceLabel50[1])
            resistanceLine50 := na
            resistanceLabel50 := na
        if not na(supportLine100)
            line.delete(supportLine100[1])
            label.delete(supportLabel100[1])
            supportLine100 := na
            supportLabel100 := na
        if not na(resistanceLine100)
            line.delete(resistanceLine100[1])
            label.delete(resistanceLabel100[1])
            resistanceLine100 := na
            resistanceLabel100 := na
        if not na(supportLine250)
            line.delete(supportLine250[1])
            label.delete(supportLabel250[1])
            supportLine250 := na
            supportLabel250 := na
        if not na(resistanceLine250)
            line.delete(resistanceLine250[1])
            label.delete(resistanceLabel250[1])
            resistanceLine250 := na
            resistanceLabel250 := na
        if orderBlocks.size() > 0
            for i = orderBlocks.size() - 1 to 0 by 1
                ob = orderBlocks.get(i)
                box.delete(ob.box)
                orderBlocks.remove(i)
        if fairValueGaps.size() > 0
            for i = 0 to fairValueGaps.size() - 1
                var fairValueGap fvg = fairValueGaps.get(i)
                if fvg.isVisible
                    box.delete(fvg.topBox)
                    box.delete(fvg.bottomBox)
            while fairValueGaps.size() > 0
                fairValueGaps.remove(0)
        if not na(fibLine1)
            line.delete(fibLine1[1])
            label.delete(fibLabel1[1])
            fibLine1 := na
            fibLabel1 := na
        if not na(fibLine2)
            line.delete(fibLine2[1])
            label.delete(fibLabel2[1])
            fibLine2 := na
            fibLabel2 := na
        if not na(fibLine3)
            line.delete(fibLine3[1])
            label.delete(fibLabel3[1])
            fibLine3 := na
            fibLabel3 := na
        if not na(fibLine4)
            line.delete(fibLine4[1])
            label.delete(fibLabel4[1])
            fibLine4 := na
            fibLabel4 := na
        if not na(fibLine5)
            line.delete(fibLine5[1])
            label.delete(fibLabel5[1])
            fibLine5 := na
            fibLabel5 := na
        if not na(fibBoxBetween50and618)
            box.delete(fibBoxBetween50and618[1])
            fibBoxBetween50and618 := na
        if not na(fibBoxBetween618and786)
            box.delete(fibBoxBetween618and786[1])
            fibBoxBetween618and786 := na
        if not na(mainTable)
            table.delete(mainTable)
            mainTable := na

if in_trade
    isLong = strategy.position_size > 0
    trailDistance = stopDistance * trailMultiplier
    trailingStop := (enableBacktest and useTrailingStopInBacktest) or (not enableBacktest and useTrailingStop) ? updateTrailingStop(isLong, entry_price, close, trailingStop, trailDistance) : trailingStop
    trailingActive := (enableBacktest and useTrailingStopInBacktest) or (not enableBacktest and useTrailingStop)
    if (enableBacktest and usePyramidingInBacktest and usePyramiding) or (not enableBacktest and usePyramiding)
        if canAddPosition(isLong, entry_price, close, pyramidPositionCount)
            pyramidPositionCount := pyramidPositionCount + 1
            newLotSize = calculatePyramidSize(lot_size, pyramidPositionCount)
            if isLong
                strategy.entry("Long Pyramid " + str.tostring(pyramidPositionCount), strategy.long, qty=newLotSize)
                if enableBacktest
                    backtestTradeCount := backtestTradeCount + 1
            else
                strategy.entry("Short Pyramid " + str.tostring(pyramidPositionCount), strategy.short, qty=newLotSize)
                if enableBacktest
                    backtestTradeCount := backtestTradeCount + 1
    if (isLong and close >= tp_level) or (not isLong and close <= tp_level)
        strategy.close(isLong ? "Long" : "Short", comment="TP Hit")
        if enableBacktest
            tradeDuration = bar_index - tradeStartBar
            backtestTotalTradeDuration := backtestTotalTradeDuration + tradeDuration
            backtestAverageTradeDuration := backtestTotalTradeDuration / backtestTradeCount
        in_trade := false
        entry_price := na
        entry_capital := na
        lot_size := na
        sl_level := na
        tp_level := na
        trailingStop := na
        trailingActive := false
        pyramidPositionCount := 0
        entry_time := na
        tradeStartBar := na
        tradeEntryPrice := na
        tradeTP := na
        tradeSL := na
    if (enableBacktest and useTrailingStopInBacktest) or (not enableBacktest and useTrailingStop)
        if trailingActive and isTrailingStopHit(isLong, close, trailingStop)
            strategy.close(isLong ? "Long" : "Short", comment="Trailing SL Hit")
            if enableBacktest
                tradeDuration = bar_index - tradeStartBar
                backtestTotalTradeDuration := backtestTotalTradeDuration + tradeDuration
                backtestAverageTradeDuration := backtestTotalTradeDuration / backtestTradeCount
            in_trade := false
            entry_price := na
            entry_capital := na
            lot_size := na
            sl_level := na
            tp_level := na
            trailingStop := na
            trailingActive := false
            pyramidPositionCount := 0
            entry_time := na
            tradeStartBar := na
            tradeEntryPrice := na
            tradeTP := na
            tradeSL := na
    if (isLong and close <= sl_level) or (not isLong and close >= sl_level)
        strategy.close(isLong ? "Long" : "Short", comment="SL Hit")
        if enableBacktest
            tradeDuration = bar_index - tradeStartBar
            backtestTotalTradeDuration := backtestTotalTradeDuration + tradeDuration
            backtestAverageTradeDuration := backtestTotalTradeDuration / backtestTradeCount
        in_trade := false
        entry_price := na
        entry_capital := na
        lot_size := na
        sl_level := na
        tp_level := na
        trailingStop := na
        trailingActive := false
        pyramidPositionCount := 0
        entry_time := na
        tradeStartBar := na
        tradeEntryPrice := na
        tradeTP := na
        tradeSL := na
    if not canTrade and in_trade
        strategy.close_all(comment="Trading Hours Ended or Daily Limit Reached")
        if enableBacktest
            tradeDuration = bar_index - tradeStartBar
            backtestTotalTradeDuration := backtestTotalTradeDuration + tradeDuration
            backtestAverageTradeDuration := backtestTotalTradeDuration / backtestTradeCount
        in_trade := false
        entry_price := na
        entry_capital := na
        lot_size := na
        sl_level := na
        tp_level := na
        trailingStop := na
        trailingActive := false
        pyramidPositionCount := 0
        entry_time := na
        tradeStartBar := na
        tradeEntryPrice := na
        tradeTP := na
        tradeSL := na

if in_trade and not na(tradeStartBar) and (not enableBacktest or showGraphicsInBacktest)
    if not na(entryLine)
        line.delete(entryLine[1])
        line.delete(slLine[1])
        line.delete(tpLine[1])
    if not na(entryLabel)
        label.delete(entryLabel[1])
        label.delete(slLabel[1])
        label.delete(tpLabel[1])
    if not na(slBox)
        box.delete(slBox[1])
    if not na(tpBox)
        box.delete(tpBox[1])
    entryLine := line.new(tradeStartBar, tradeEntryPrice, bar_index, tradeEntryPrice, color=color.blue, style=line.style_solid)
    slLine := line.new(tradeStartBar, tradeSL, bar_index, tradeSL, color=color.red, style=line.style_solid)
    tpLine := line.new(tradeStartBar, tradeTP, bar_index, tradeTP, color=color.green, style=line.style_solid)
    slBox := box.new(left=tradeStartBar, right=bar_index, top=tradeEntryPrice, bottom=tradeSL, border_color=color.red, bgcolor=color.new(color.red, 70), xloc=xloc.bar_index)
    tpBox := box.new(left=tradeStartBar, right=bar_index, top=tradeEntryPrice, bottom=tradeTP, border_color=color.green, bgcolor=color.new(color.green, 70), xloc=xloc.bar_index)
    float pipsDistance = 0.0
    if pair == "BTCUSD"
        pipsDistance := 400.0
    else if pair == "XAUUSD"
        pipsDistance := 35.0
    else if pair == "CHFJPY"
        pipsDistance := 30.0
    else if pair == "GBPJPY" or pair == "EURJPY" or pair == "USDJPY"
        pipsDistance := 20.0
    else if pair == "EURUSD" or pair == "GBPUSD" or pair == "USDCAD" or pair == "EURGBP"
        pipsDistance := 15.0
    entryLabelText = "Entrée: " + str.tostring(tradeEntryPrice, "#.##") + " $"
    slLabelText = "SL: " + str.tostring(tradeSL, "#.##") + " $"
    tpLabelText = "TP: " + str.tostring(tradeTP, "#.##") + " $"
    entryLabelText += "\nLot: " + str.tostring(lot_size, "#.##") + "\nCapital: " + str.tostring(entry_capital, "#.##") + " $"
    slLabelText += "\nRisque: " + str.tostring(riskAmount, "#.##") + "%\nDistance: " + str.tostring(stopDistance / valeur_pips, "#.##") + " pips"
    tpLabelText += "\nDistance: " + str.tostring(pipsDistance * tp_ratio, "#.##") + " pips\nPotentiel: " + str.tostring((tradeTP - tradeEntryPrice) / valeur_pips, "#.##") + " pips"
    entryLabel := label.new(bar_index, tradeEntryPrice, entryLabelText, color=color.new(color.blue, 70), textcolor=color.white, style=label.style_label_left)
    slLabel := label.new(bar_index, tradeSL, slLabelText, color=color.new(color.red, 70), textcolor=color.white, style=label.style_label_left)
    tpLabel := label.new(bar_index, tradeTP, tpLabelText, color=color.new(color.green, 70), textcolor=color.white, style=label.style_label_left)

if not in_trade and (not enableBacktest or showGraphicsInBacktest)
    if not na(entryLine)
        line.delete(entryLine[1])
        line.delete(slLine[1])
        line.delete(tpLine[1])
        entryLine := na
        slLine := na
        tpLine := na
    if not na(entryLabel)
        label.delete(entryLabel[1])
        label.delete(slLabel[1])
        label.delete(tpLabel[1])
        entryLabel := na
        slLabel := na
        tpLabel := na
    if not na(slBox)
        box.delete(slBox[1])
        slBox := na
    if not na(tpBox)
        box.delete(tpBox[1])
        tpBox := na

float adxPlotValue = (not enableBacktest or showGraphicsInBacktest) ? (useADX ? adxNormalized : na) : na
float diPlusPlotValue = (not enableBacktest or showGraphicsInBacktest) ? (useADX ? diPlusNormalized : na) : na
float diMinusPlotValue = (not enableBacktest or showGraphicsInBacktest) ? (useADX ? diMinusNormalized : na) : na
float macdLinePlotValue = (not enableBacktest or showGraphicsInBacktest) ? (useMACD ? macdLineNormalized : na) : na
float signalLinePlotValue = (not enableBacktest or showGraphicsInBacktest) ? (useMACD ? signalLineNormalized : na) : na
float obvPlotValue = (not enableBacktest or showGraphicsInBacktest) ? (useOBV ? obvNormalized : na) : na
float obvMAPlotValue = (not enableBacktest or showGraphicsInBacktest) ? (useOBV ? obvMANormalized : na) : na
float upperBBPlotValue = (not enableBacktest or showGraphicsInBacktest) ? (useSqueeze ? upperBB : na) : na
float basisPlotValue = (not enableBacktest or showGraphicsInBacktest) ? (useSqueeze ? basis : na) : na
float lowerBBPlotValue = (not enableBacktest or showGraphicsInBacktest) ? (useSqueeze ? lowerBB : na) : na
float upperKCPlotValue = (not enableBacktest or showGraphicsInBacktest) ? (useSqueeze ? upperKC : na) : na
float maPlotValue = (not enableBacktest or showGraphicsInBacktest) ? (useSqueeze ? ma : na) : na
float lowerKCPlotValue = (not enableBacktest or showGraphicsInBacktest) ? (useSqueeze ? lowerKC : na) : na
float stmaUpPlotValue = (not enableBacktest or showGraphicsInBacktest) ? (useSTMA and stmaTrend == 1 ? stmaUp : na) : na
float stmaDnPlotValue = (not enableBacktest or showGraphicsInBacktest) ? (useSTMA and stmaTrend == -1 ? stmaDn : na) : na
float stmaMPlotValue = (not enableBacktest or showGraphicsInBacktest) ? (useSTMA ? ohlc4 : na) : na
color stmaLongFillColorValue = (not enableBacktest or showGraphicsInBacktest) ? (useSTMA and stmaTrend == 1 ? color.new(color.green, 75) : color.new(color.white, 100)) : color.new(color.white, 100)
color stmaShortFillColorValue = (not enableBacktest or showGraphicsInBacktest) ? (useSTMA and stmaTrend == -1 ? color.new(color.red, 75) : color.new(color.white, 100)) : color.new(color.white, 100)

var adxPlotId = plot(adxPlotValue, "ADX", color.purple, 1)
var diPlusPlotId = plot(diPlusPlotValue, "DI+", color.green, 1)
var diMinusPlotId = plot(diMinusPlotValue, "DI-", color.red, 1)
fill(diPlusPlotId, diMinusPlotId, title="DI Fill", color=diPlus > diMinus ? color.new(color.blue, 70) : color.new(color.navy, 70))
var macdLinePlotId = plot(macdLinePlotValue, "MACD Line", color.new(color.gray, 20), 1)
var signalLinePlotId = plot(signalLinePlotValue, "Signal Line", color.new(color.yellow, 20), 1)
fill(macdLinePlotId, signalLinePlotId, title="MACD Fill", color=macdLine > signalLine ? color.new(color.green, 70) : color.new(color.red, 70))
var obvPlotId = plot(obvPlotValue, "OBV", color.new(color.rgb(0, 255, 255), 20), 1)
var obvMAPlotId = plot(obvMAPlotValue, "OBV MA", color.new(color.rgb(255, 0, 255), 20), 1)
fill(obvPlotId, obvMAPlotId, title="OBV Fill", color=obv > obvMA ? color.new(color.yellow, 70) : color.new(color.black, 70))
var upperBBPlotId = plot(upperBBPlotValue, "BB Upper", color.new(color.blue, 20), 1, style=plot.style_line)
plot(basisPlotValue, "BB Middle", color.new(color.blue, 20), 1, style=plot.style_line)
var lowerBBPlotId = plot(lowerBBPlotValue, "BB Lower", color.new(color.blue, 20), 1, style=plot.style_line)
var upperKCPlotId = plot(upperKCPlotValue, "KC Upper", color.new(color.orange, 20), 1, style=plot.style_line)
plot(maPlotValue, "KC Middle", color.new(color.orange, 20), 1, style=plot.style_line)
var lowerKCPlotId = plot(lowerKCPlotValue, "KC Lower", color.new(color.orange, 20), 1, style=plot.style_line)
fill(upperBBPlotId, lowerBBPlotId, title="BB Fill", color=squeezeOn ? color.new(color.purple, 80) : color.new(color.blue, 80))
fill(upperKCPlotId, lowerKCPlotId, title="KC Fill", color=squeezeOn ? color.new(color.purple, 80) : color.new(color.orange, 80))

var stmaMPlotId = plot(stmaMPlotValue, "SuperTrend Middle", color.gray, 1, style=plot.style_circles)
var stmaUpPlotId = plot(stmaUpPlotValue, "SuperTrend Up", color.green, 1, style=plot.style_linebr)
var stmaDnPlotId = plot(stmaDnPlotValue, "SuperTrend Down", color.red, 1, style=plot.style_linebr)
fill(stmaMPlotId, stmaUpPlotId, title="SuperTrend Haussier", color=stmaLongFillColorValue)
fill(stmaMPlotId, stmaDnPlotId, title="SuperTrend Baissier", color=stmaShortFillColorValue)

varip bool isFirstBarConfirmed = false
if ta.barssince(timeframe.change("")) == 0 and not isFirstBarConfirmed
    isFirstBarConfirmed := true

bool noIndicatorsActive = not useSTMA and not useRSI and not useADX and not useMACD and not useOBV and not useSqueeze and not useCandlePatternsInConditions and not useSR and not useOB and not useFVG and not useFibonacci and not useStoch and not useDivergence and not useBreakout

// TABLEAU DES INDICATEURS
if isConfirmed and showIndicatorTable and (not enableBacktest or showGraphicsInBacktest)
    if not na(mainTable)
        table.delete(mainTable)
    int validNumCols = math.max(tableNumCols, 1)
    int validNumRows = math.max(tableNumRows, 1)
    mainTable := table.new(position.top_right, validNumCols, validNumRows, border_width=1, border_color=color.white)
    table.cell(mainTable, 0, 0, enableBacktest ? "BACKTEST ACTIF" : "Conditions", bgcolor=enableBacktest ? color.new(color.red, 90) : color.new(color.black, 90), text_color=color.white, text_size=size.normal)
    table.cell(mainTable, 1, 0, "Indicateurs", bgcolor=color.new(color.black, 90), text_color=color.white, text_size=size.normal)
    table.cell(mainTable, 2, 0, "Analyse", bgcolor=color.new(color.black, 90), text_color=color.white, text_size=size.normal)

    // Calculs communs pour le scalping
    float smaDistancePips = (close - longTermSMA) / valeur_pips
    float stmaDistancePips = stmaTrend == 1 ? (close - stmaUp) / valeur_pips : (close - stmaDn) / valeur_pips
    float atrPips = currentATR / valeur_pips
    float obvGapPips = useOBV ? (obv - obvMA) / valeur_pips : na
    float volumeRatio = volume / avgVolume
    float momentumChange = useStoch ? (stochK - stochK[2]) : na
    float fibDistancePips = not na(fibClosestLevelValue) ? (close - fibClosestLevelValue) / valeur_pips : na
    float srDistancePips = not na(closestLevelPrice) ? (close - closestLevelPrice) / valeur_pips : na
    float fvgTopDistancePips = not na(topFVG) ? (close - topFVG) / valeur_pips : na
    float fvgBottomDistancePips = not na(bottomFVG) ? (close - bottomFVG) / valeur_pips : na
    float macdGapPips = useMACD ? (macdLine - signalLine) / valeur_pips : na
    float diGapPips = useADX ? (diPlus - diMinus) / valeur_pips : na
    float obDistancePips = lastBullishOBDetected ? (close - bottomOBBullish) / valeur_pips : lastBearishOBDetected ? (close - topOBBearish) / valeur_pips : na

    // Compteur des indicateurs activés
    int activeIndicators = 0
    activeIndicators := activeIndicators + (useSTMA ? 1 : 0)
    activeIndicators := activeIndicators + ((useRSI or useStoch) ? 1 : 0)
    activeIndicators := activeIndicators + (useDivergence and useRSI ? 1 : 0)
    activeIndicators := activeIndicators + (useADX ? 1 : 0)
    activeIndicators := activeIndicators + (useMACD ? 1 : 0)
    activeIndicators := activeIndicators + (useOBV ? 1 : 0)
    activeIndicators := activeIndicators + (useSqueeze ? 1 : 0)
    activeIndicators := activeIndicators + (useCandlePatternsInConditions ? 1 : 0)
    activeIndicators := activeIndicators + (useFVG ? 1 : 0)
    activeIndicators := activeIndicators + (useFibonacci ? 1 : 0)
    activeIndicators := activeIndicators + (useSR ? 1 : 0)
    activeIndicators := activeIndicators + (useBreakout ? 1 : 0)
    activeIndicators := activeIndicators + (useOB ? 1 : 0)

    trendScore := 0
    string smaAnalysis = ""
    color smaBgColor = color.gray
    if close > longTermSMA
        smaAnalysis := "Hausse.\nPrix +" + str.tostring(smaDistancePips, "#.##") + " pips au-dessus SMA(" + str.tostring(longTermTrendLength) + ").\nSupport: " + str.tostring(supportLevel, "#.##") + " $.\nScalp si rebond."
        smaBgColor := color.new(color.green, 70)
        trendScore := trendScore + 1
    else if close < longTermSMA
        smaAnalysis := "Baisse.\nPrix " + str.tostring(smaDistancePips, "#.##") + " pips sous SMA(" + str.tostring(longTermTrendLength) + ").\nRésistance: " + str.tostring(resistanceLevel, "#.##") + " $.\nScalp si rebond."
        smaBgColor := color.new(color.red, 70)
        trendScore := trendScore - 1
    else
        smaAnalysis := "Neutre.\nPrix proche de SMA(" + str.tostring(longTermTrendLength) + ").\nSupport: " + str.tostring(supportLevel, "#.##") + " $.\nScalp si cassure."
        smaBgColor := color.new(color.gray, 90)
    table.cell(mainTable, 0, 1, close > longTermSMA or close < longTermSMA ? "✅" : "❌", bgcolor=smaBgColor, text_color=color.white, text_size=size.normal)
    table.cell(mainTable, 1, 1, "SMA Long Terme", bgcolor=smaBgColor, text_color=color.white, text_size=size.normal)
    table.cell(mainTable, 2, 1, smaAnalysis, bgcolor=smaBgColor, text_color=color.white, text_size=size.normal)

    string stmaAnalysis = ""
    if useSTMA
        if stmaTrend == 1
            stmaAnalysis := "Hausse.\nPrix +" + str.tostring(stmaDistancePips, "#.##") + " pips au-dessus STMA.\nVolatilité: " + str.tostring(atrPips, "#.##") + " pips.\nProchain niveau: " + (not na(topFVG) ? str.tostring(topFVG, "#.##") : str.tostring(resistanceLevel, "#.##")) + " $.\nScalp si cassure."
            trendScore := trendScore + 1
        else
            stmaAnalysis := "Baisse.\nPrix " + str.tostring(stmaDistancePips, "#.##") + " pips sous STMA.\nVolatilité: " + str.tostring(atrPips, "#.##") + " pips.\nProchain niveau: " + (not na(bottomFVG) ? str.tostring(bottomFVG, "#.##") : str.tostring(supportLevel, "#.##")) + " $.\nScalp si cassure."
            trendScore := trendScore - 1
    else
        stmaAnalysis := "Analyse la tendance principale.\nVolatilité: " + str.tostring(atrPips, "#.##") + " pips.\nProchain niveau: " + str.tostring(resistanceLevel, "#.##") + " $.\nActiver pour scalping."
    table.cell(mainTable, 0, 2, useSTMA and (stmaLongCondition or stmaShortCondition) ? "✅" : "❌", bgcolor=color.new(color.black, 90), text_color=color.white, text_size=size.normal)
    table.cell(mainTable, 1, 2, "SuperTrend", bgcolor=color.new(color.black, 90), text_color=useSTMA ? color.green : color.red, text_size=size.normal)
    table.cell(mainTable, 2, 2, stmaAnalysis, bgcolor=color.new(color.black, 90), text_color=color.white, text_size=size.normal)

    string momentumAnalysis = ""
    bool momentumActive = useRSI or useStoch
    if momentumActive
        if (useRSI and rsiValue > 65) or (useStoch and stochK < momentumThreshold)
            momentumAnalysis := "Baisse.\nRSI: " + (useRSI ? str.tostring(rsiValue, "#.##") + " (>65)" : "N/A") + ".\nStoch %K: " + (useStoch ? str.tostring(stochK, "#.##") + " (+" + str.tostring(momentumChange, "#.##") + "%)" : "N/A") + ".\nMomentum baissier.\nScalp si breakout."
            trendScore := trendScore - 1
        else if (useRSI and rsiValue < 35) or (useStoch and stochK > momentumThreshold)
            momentumAnalysis := "Hausse.\nRSI: " + (useRSI ? str.tostring(rsiValue, "#.##") + " (<35)" : "N/A") + ".\nStoch %K: " + (useStoch ? str.tostring(stochK, "#.##") + " (+" + str.tostring(momentumChange, "#.##") + "%)" : "N/A") + ".\nMomentum haussier.\nScalp si breakout."
            trendScore := trendScore + 1
        else if (useRSI and rsiValue > momentumThreshold) or (useStoch and stochK > momentumThreshold)
            momentumAnalysis := "Hausse.\nRSI: " + (useRSI ? str.tostring(rsiValue, "#.##") + " (>50)" : "N/A") + ".\nStoch %K: " + (useStoch ? str.tostring(stochK, "#.##") + " (+" + str.tostring(momentumChange, "#.##") + "%)" : "N/A") + ".\nMomentum haussier.\nScalp si breakout."
        else if (useRSI and rsiValue < momentumThreshold) or (useStoch and stochK < momentumThreshold)
            momentumAnalysis := "Baisse.\nRSI: " + (useRSI ? str.tostring(rsiValue, "#.##") + " (<50)" : "N/A") + ".\nStoch %K: " + (useStoch ? str.tostring(stochK, "#.##") + " (+" + str.tostring(momentumChange, "#.##") + "%)" : "N/A") + ".\nMomentum baissier.\nScalp si breakout."
        else
            momentumAnalysis := "Neutre.\nRSI: " + (useRSI ? str.tostring(rsiValue, "#.##") : "N/A") + ".\nStoch %K: " + (useStoch ? str.tostring(stochK, "#.##") : "N/A") + ".\nPas de signal clair.\nScalp si cassure."
    else
        momentumAnalysis := "Analyse momentum.\nVolatilité: " + str.tostring(atrPips, "#.##") + " pips.\nProchain niveau: " + str.tostring(resistanceLevel, "#.##") + " $.\nActiver RSI/Stoch."
    table.cell(mainTable, 0, 3, momentumActive and ((useRSI and (rsiValue > 65 or rsiValue < 35 or rsiValue > momentumThreshold or rsiValue < momentumThreshold)) or (useStoch and (stochK > momentumThreshold or stochK < momentumThreshold))) ? "✅" : "❌", bgcolor=color.new(color.black, 90), text_color=color.white, text_size=size.normal)
    table.cell(mainTable, 1, 3, "Momentum (RSI/Stoch)", bgcolor=color.new(color.black, 90), text_color=momentumActive ? color.green : color.red, text_size=size.normal)
    table.cell(mainTable, 2, 3, momentumAnalysis, bgcolor=color.new(color.black, 90), text_color=color.white, text_size=size.normal)

    string divAnalysis = ""
    if useDivergence and useRSI
        if bullishDivergence
            divAnalysis := "Hausse.\nDivergence haussière.\nPrix bas: " + str.tostring(priceLow, "#.##") + ", RSI: " + str.tostring(rsiLow, "#.##") + ".\nSupport: " + str.tostring(supportLevel, "#.##") + " $.\nScalp si volume."
            trendScore := trendScore + 1
        else if bearishDivergence
            divAnalysis := "Baisse.\nDivergence baissière.\nPrix haut: " + str.tostring(priceHigh, "#.##") + ", RSI: " + str.tostring(rsiHigh, "#.##") + ".\nRésistance: " + str.tostring(resistanceLevel, "#.##") + " $.\nScalp si volume."
            trendScore := trendScore - 1
        else
            divAnalysis := "Neutre.\nPas de divergence.\nProchain niveau: " + str.tostring(supportLevel, "#.##") + " $.\nScalp si cassure."
    else
        divAnalysis := "Détecte divergences.\nVolatilité: " + str.tostring(atrPips, "#.##") + " pips.\nProchain niveau: " + str.tostring(supportLevel, "#.##") + " $.\nActiver pour scalping."
    table.cell(mainTable, 0, 4, useDivergence and useRSI and (bullishDivergence or bearishDivergence) ? "✅" : "❌", bgcolor=color.new(color.black, 90), text_color=color.white, text_size=size.normal)
    table.cell(mainTable, 1, 4, "Divergence", bgcolor=color.new(color.black, 90), text_color=(useDivergence and useRSI) ? color.green : color.red, text_size=size.normal)
    table.cell(mainTable, 2, 4, divAnalysis, bgcolor=color.new(color.black, 90), text_color=color.white, text_size=size.normal)

    string adxAnalysis = ""
    if useADX
        if adxBullish and adxTrending
            adxAnalysis := "Hausse.\nADX: " + str.tostring(adx, "#.##") + " (>20).\nDI+: " + str.tostring(diPlus, "#.##") + ", DI-: " + str.tostring(diMinus, "#.##") + " (+" + str.tostring(diGapPips, "#.##") + " pips).\nMicro-tendance haussière.\nScalp si breakout."
            trendScore := trendScore + 1
        else if adxBearish and adxTrending
            adxAnalysis := "Baisse.\nADX: " + str.tostring(adx, "#.##") + " (>20).\nDI+: " + str.tostring(diPlus, "#.##") + ", DI-: " + str.tostring(diMinus, "#.##") + " (" + str.tostring(diGapPips, "#.##") + " pips).\nMicro-tendance baissière.\nScalp si breakout."
            trendScore := trendScore - 1
        else
            adxAnalysis := "Neutre.\nADX: " + str.tostring(adx, "#.##") + ".\nDI+: " + str.tostring(diPlus, "#.##") + ", DI-: " + str.tostring(diMinus, "#.##") + ".\nProchain niveau: " + str.tostring(resistanceLevel, "#.##") + " $.\nScalp si cassure."
    else
        adxAnalysis := "Mesure la tendance.\nVolatilité: " + str.tostring(atrPips, "#.##") + " pips.\nProchain niveau: " + str.tostring(resistanceLevel, "#.##") + " $.\nActiver pour scalping."
    table.cell(mainTable, 0, 5, useADX and (adxLongCondition or adxShortCondition) ? "✅" : "❌", bgcolor=color.new(color.black, 90), text_color=color.white, text_size=size.normal)
    table.cell(mainTable, 1, 5, "ADX", bgcolor=color.new(color.black, 90), text_color=useADX ? color.green : color.red, text_size=size.normal)
    table.cell(mainTable, 2, 5, adxAnalysis, bgcolor=color.new(color.black, 90), text_color=color.white, text_size=size.normal)

    string macdAnalysis = ""
    if useMACD
        if macdCrossUp and macdLine > 0
            macdAnalysis := "Hausse.\nCroisement haussier.\nMACD: " + str.tostring(macdLine, "#.##") + ", Signal: " + str.tostring(signalLine, "#.##") + " (+" + str.tostring(macdGapPips, "#.##") + " pips).\nProchain niveau: " + str.tostring(resistanceLevel, "#.##") + " $.\nScalp si cassure."
            trendScore := trendScore + 1
        else if macdCrossDown and macdLine < 0
            macdAnalysis := "Baisse.\nCroisement baissier.\nMACD: " + str.tostring(macdLine, "#.##") + ", Signal: " + str.tostring(signalLine, "#.##") + " (" + str.tostring(macdGapPips, "#.##") + " pips).\nProchain niveau: " + str.tostring(supportLevel, "#.##") + " $.\nScalp si cassure."
            trendScore := trendScore - 1
        else
            macdAnalysis := "Neutre.\nMACD: " + str.tostring(macdLine, "#.##") + ", Signal: " + str.tostring(signalLine, "#.##") + ".\nProchain niveau: " + str.tostring(resistanceLevel, "#.##") + " $.\nScalp si cassure."
    else
        macdAnalysis := "Identifie momentum.\nVolatilité: " + str.tostring(atrPips, "#.##") + " pips.\nProchain niveau: " + str.tostring(resistanceLevel, "#.##") + " $.\nActiver pour scalping."
    table.cell(mainTable, 0, 6, useMACD and (macdLongCondition or macdShortCondition) ? "✅" : "❌", bgcolor=color.new(color.black, 90), text_color=color.white, text_size=size.normal)
    table.cell(mainTable, 1, 6, "MACD", bgcolor=color.new(color.black, 90), text_color=useMACD ? color.green : color.red, text_size=size.normal)
    table.cell(mainTable, 2, 6, macdAnalysis, bgcolor=color.new(color.black, 90), text_color=color.white, text_size=size.normal)

    string obvAnalysis = ""
    if useOBV
        if obvRising
            obvAnalysis := "Hausse.\nOBV > MA (+" + str.tostring(obvGapPips, "#.##") + " pips).\nVolume: " + str.tostring(volumeRatio, "#.##") + "x moyenne.\nSupport: " + str.tostring(supportLevel, "#.##") + " $.\nScalp si rebond."
            trendScore := trendScore + 1
        else if obvFalling
            obvAnalysis := "Baisse.\nOBV < MA (" + str.tostring(obvGapPips, "#.##") + " pips).\nVolume: " + str.tostring(volumeRatio, "#.##") + "x moyenne.\nRésistance: " + str.tostring(resistanceLevel, "#.##") + " $.\nScalp si rebond."
            trendScore := trendScore - 1
        else
            obvAnalysis := "Neutre.\nOBV stable.\nVolume: " + str.tostring(volumeRatio, "#.##") + "x moyenne.\nProchain niveau: " + str.tostring(resistanceLevel, "#.##") + " $.\nScalp si cassure."
    else
        obvAnalysis := "Analyse volume.\nVolume: " + str.tostring(volumeRatio, "#.##") + "x moyenne.\nProchain niveau: " + str.tostring(resistanceLevel, "#.##") + " $.\nActiver pour scalping."
    table.cell(mainTable, 0, 7, useOBV and (obvLongCondition or obvShortCondition) ? "✅" : "❌", bgcolor=color.new(color.black, 90), text_color=color.white, text_size=size.normal)
    table.cell(mainTable, 1, 7, "OBV", bgcolor=color.new(color.black, 90), text_color=useOBV ? color.green : color.red, text_size=size.normal)
    table.cell(mainTable, 2, 7, obvAnalysis, bgcolor=color.new(color.black, 90), text_color=color.white, text_size=size.normal)

    string squeezeAnalysis = ""
    if useSqueeze
        if squeezeOff and mom > minMomentum * avgRange
            squeezeAnalysis := "Hausse.\nSqueeze Off.\nMomentum: " + str.tostring(mom / valeur_pips, "#.##") + " pips (> " + str.tostring(minMomentum * avgRange / valeur_pips, "#.##") + ").\nProchain niveau: " + str.tostring(resistanceLevel, "#.##") + " $.\nScalp si breakout."
            trendScore := trendScore + 1
        else if squeezeOff and mom < -minMomentum * avgRange
            squeezeAnalysis := "Baisse.\nSqueeze Off.\nMomentum: " + str.tostring(mom / valeur_pips, "#.##") + " pips (< " + str.tostring(-minMomentum * avgRange / valeur_pips, "#.##") + ").\nProchain niveau: " + str.tostring(supportLevel, "#.##") + " $.\nScalp si breakout."
            trendScore := trendScore - 1
        else
            squeezeAnalysis := "Neutre.\nCompression en cours.\nVolatilité: " + str.tostring(squeezeVolatilitySma / valeur_pips, "#.##") + " pips.\nProchain niveau: " + str.tostring(resistanceLevel, "#.##") + " $.\nScalp si cassure."
    else
        squeezeAnalysis := "Repère impulsions.\nVolatilité: " + str.tostring(atrPips, "#.##") + " pips.\nProchain niveau: " + str.tostring(resistanceLevel, "#.##") + " $.\nActiver pour scalping."
    table.cell(mainTable, 0, 8, useSqueeze and (squeezeLongCondition or squeezeShortCondition) ? "✅" : "❌", bgcolor=color.new(color.black, 90), text_color=color.white, text_size=size.normal)
    table.cell(mainTable, 1, 8, "Squeeze", bgcolor=color.new(color.black, 90), text_color=useSqueeze ? color.green : color.red, text_size=size.normal)
    table.cell(mainTable, 2, 8, squeezeAnalysis, bgcolor=color.new(color.black, 90), text_color=color.white, text_size=size.normal)

    string candleAnalysis = ""
    if useCandlePatternsInConditions
        if candlePattern != ""
            if bullishEngulfing or hammer or invertedHammer or pinBarBullish or marubozuUp
                candleAnalysis := "Hausse.\nMotif: " + candlePattern + ".\nVolatilité: " + str.tostring(candleVolatilitySma / valeur_pips, "#.##") + " pips.\nSupport: " + str.tostring(supportLevel, "#.##") + " $.\nScalp si cassure " + str.tostring(topFVG, "#.##") + " $."
                trendScore := trendScore + 1
            else if bearishEngulfing or shootingStar or pinBarBearish or marubozuDown
                candleAnalysis := "Baisse.\nMotif: " + candlePattern + ".\nVolatilité: " + str.tostring(candleVolatilitySma / valeur_pips, "#.##") + " pips.\nRésistance: " + str.tostring(resistanceLevel, "#.##") + " $.\nScalp si cassure " + str.tostring(bottomFVG, "#.##") + " $."
                trendScore := trendScore - 1
            else
                candleAnalysis := "Neutre.\nMotif: " + candlePattern + ".\nVolatilité: " + str.tostring(candleVolatilitySma / valeur_pips, "#.##") + " pips.\nProchain niveau: " + str.tostring(resistanceLevel, "#.##") + " $.\nScalp si cassure."
        else
            candleAnalysis := "Aucun motif.\nVolatilité: " + str.tostring(candleVolatilitySma / valeur_pips, "#.##") + " pips.\nProchain niveau: " + str.tostring(resistanceLevel, "#.##") + " $.\nScalp si cassure."
    else
        candleAnalysis := "Détecte motifs.\nVolatilité: " + str.tostring(atrPips, "#.##") + " pips.\nProchain niveau: " + str.tostring(resistanceLevel, "#.##") + " $.\nActiver pour scalping."
    table.cell(mainTable, 0, 9, useCandlePatternsInConditions and (candleLongCondition or candleShortCondition) ? "✅" : "❌", bgcolor=color.new(color.black, 90), text_color=color.white, text_size=size.normal)
    table.cell(mainTable, 1, 9, "Candle", bgcolor=color.new(color.black, 90), text_color=useCandlePatternsInConditions ? color.green : color.red, text_size=size.normal)
    table.cell(mainTable, 2, 9, candleAnalysis, bgcolor=color.new(color.black, 90), text_color=color.white, text_size=size.normal)

    string fvgAnalysis = ""
    color fvgTextColor = color.white
    if useFVG
        if lastBullishFVGDetected
            if lastImbalanceType == "FVG"
                fvgAnalysis := "Hausse.\nFVG Haussier.\nTaille: " + str.tostring((topFVG - bottomFVG) / valeur_pips, "#.##") + " pips.\nPrix dans zone [" + str.tostring(bottomFVG, "#.##") + "-" + str.tostring(topFVG, "#.##") + "].\nCible: " + str.tostring(resistanceLevel, "#.##") + " $.\nScalp si volume."
                fvgTextColor := color.white
                trendScore := trendScore + 1
            else if lastImbalanceType == "Consecutive"
                fvgAnalysis := "Hausse.\nImbalance consécutive haussière.\nTaille: " + str.tostring((topFVG - bottomFVG) / valeur_pips, "#.##") + " pips.\nPrix dans zone [" + str.tostring(bottomFVG, "#.##") + "-" + str.tostring(topFVG, "#.##") + "].\nCible: " + str.tostring(resistanceLevel, "#.##") + " $.\nScalp si volume."
                fvgTextColor := color.blue
                trendScore := trendScore + 0.5
            else if lastImbalanceType == "Liquidity"
                fvgAnalysis := "Hausse.\nImbalance de liquidité.\nTaille: " + str.tostring((topFVG - bottomFVG) / valeur_pips, "#.##") + " pips.\nPrix dans zone [" + str.tostring(bottomFVG, "#.##") + "-" + str.tostring(topFVG, "#.##") + "].\nCible: " + str.tostring(resistanceLevel, "#.##") + " $.\nScalp si volume."
                fvgTextColor := color.new(#006400, 0)
                trendScore := trendScore + 1.5
        else if lastBearishFVGDetected
            if lastImbalanceType == "FVG"
                fvgAnalysis := "Baisse.\nFVG Baissier.\nTaille: " + str.tostring((topFVG - bottomFVG) / valeur_pips, "#.##") + " pips.\nPrix dans zone [" + str.tostring(bottomFVG, "#.##") + "-" + str.tostring(topFVG, "#.##") + "].\nCible: " + str.tostring(supportLevel, "#.##") + " $.\nScalp si volume."
                fvgTextColor := color.white
                trendScore := trendScore - 1
            else if lastImbalanceType == "Consecutive"
                fvgAnalysis := "Baisse.\nImbalance consécutive baissière.\nTaille: " + str.tostring((topFVG - bottomFVG) / valeur_pips, "#.##") + " pips.\nPrix dans zone [" + str.tostring(bottomFVG, "#.##") + "-" + str.tostring(topFVG, "#.##") + "].\nCible: " + str.tostring(supportLevel, "#.##") + " $.\nScalp si volume."
                fvgTextColor := color.purple
                trendScore := trendScore - 0.5
            else if lastImbalanceType == "Liquidity"
                fvgAnalysis := "Baisse.\nImbalance de liquidité.\nTaille: " + str.tostring((topFVG - bottomFVG) / valeur_pips, "#.##") + " pips.\nPrix dans zone [" + str.tostring(bottomFVG, "#.##") + "-" + str.tostring(topFVG, "#.##") + "].\nCible: " + str.tostring(supportLevel, "#.##") + " $.\nScalp si volume."
                fvgTextColor := color.new(#8B0000, 0)
                trendScore := trendScore - 1.5
        else
            fvgAnalysis := "Aucun FVG.\nPrix hors zone.\nTop FVG: " + str.tostring(fvgTopDistancePips, "#.##") + " pips.\nBottom FVG: " + str.tostring(fvgBottomDistancePips, "#.##") + " pips.\nScalp si entrée zone."
    else
        fvgAnalysis := "Détecte déséquilibres.\nVolatilité: " + str.tostring(atrPips, "#.##") + " pips.\nProchain niveau: " + str.tostring(resistanceLevel, "#.##") + " $.\nActiver pour scalping."
    table.cell(mainTable, 0, 10, useFVG and (fvgLongCondition or fvgShortCondition) ? "✅" : "❌", bgcolor=color.new(color.black, 90), text_color=color.white, text_size=size.normal)
    table.cell(mainTable, 1, 10, "FVG", bgcolor=color.new(color.black, 90), text_color=useFVG ? color.green : color.red, text_size=size.normal)
    table.cell(mainTable, 2, 10, fvgAnalysis, bgcolor=color.new(color.black, 90), text_color=fvgTextColor, text_size=size.normal)

    string fibAnalysis = ""
    if useFibonacci
        if not na(fibClosestLevel)
            if nearFibLevelBullish and (fibClosestLevel == float(fibLevel3) or fibClosestLevel == float(fibLevel4))
                fibAnalysis := "Hausse.\nNiveau: " + fibClosestLevelStr + " (" + str.tostring(fibClosestLevelValue, "#.##") + " $).\nPrix: " + str.tostring(close, "#.##") + " (" + str.tostring(fibDistancePips, "#.##") + " pips).\nCible: 78.6% (" + str.tostring(fibLevel2Price, "#.##") + " $).\nScalp si rebond."
                trendScore := trendScore + 1
            else if nearFibLevelBearish and (fibClosestLevel == float(fibLevel3) or fibClosestLevel == float(fibLevel4))
                fibAnalysis := "Baisse.\nNiveau: " + fibClosestLevelStr + " (" + str.tostring(fibClosestLevelValue, "#.##") + " $).\nPrix: " + str.tostring(close, "#.##") + " (" + str.tostring(fibDistancePips, "#.##") + " pips).\nCible: 50% (" + str.tostring(fibLevel4Price, "#.##") + " $).\nScalp si rebond."
                trendScore := trendScore - 1
            else
                fibAnalysis := "Neutre.\nNiveau: " + fibClosestLevelStr + " (" + str.tostring(fibClosestLevelValue, "#.##") + " $).\nPrix: " + str.tostring(close, "#.##") + " (" + str.tostring(fibDistancePips, "#.##") + " pips).\nProchain niveau: " + str.tostring(resistanceLevel, "#.##") + " $.\nScalp si cassure."
        else
            fibAnalysis := "Aucun niveau.\nVolatilité: " + str.tostring(atrPips, "#.##") + " pips.\nProchain niveau: " + str.tostring(resistanceLevel, "#.##") + " $.\nScalp si cassure."
    else
        fibAnalysis := "Détecte retracements.\nVolatilité: " + str.tostring(atrPips, "#.##") + " pips.\nProchain niveau: " + str.tostring(resistanceLevel, "#.##") + " $.\nActiver pour scalping."
    table.cell(mainTable, 0, 11, useFibonacci and (fibLongCondition or fibShortCondition) ? "✅" : "❌", bgcolor=color.new(color.black, 90), text_color=color.white, text_size=size.normal)
    table.cell(mainTable, 1, 11, "Fibonacci", bgcolor=color.new(color.black, 90), text_color=useFibonacci ? color.green : color.red, text_size=size.normal)
    table.cell(mainTable, 2, 11, fibAnalysis, bgcolor=color.new(color.black, 90), text_color=color.white, text_size=size.normal)

    string srAnalysis = ""
    if useSR
        if not na(closestLevelPrice)
            if srLongCondition
                srAnalysis := "Hausse.\nSupport: " + str.tostring(closestLevelPrice, "#.##") + " $.\nPrix: " + str.tostring(close, "#.##") + " (+" + str.tostring(srDistancePips, "#.##") + " pips).\nTests: " + str.tostring(testCountSupport10) + ".\nCible: " + str.tostring(resistanceLevel, "#.##") + " $.\nScalp si rebond."
                trendScore := trendScore + 1
            else if srShortCondition
                srAnalysis := "Baisse.\nRésistance: " + str.tostring(closestLevelPrice, "#.##") + " $.\nPrix: " + str.tostring(close, "#.##") + " (" + str.tostring(srDistancePips, "#.##") + " pips).\nTests: " + str.tostring(testCountResistance10) + ".\nCible: " + str.tostring(supportLevel, "#.##") + " $.\nScalp si rebond."
                trendScore := trendScore - 1
            else
                srAnalysis := "Neutre.\nProche niveau: " + str.tostring(closestLevelPrice, "#.##") + " $.\nPrix: " + str.tostring(close, "#.##") + " (" + str.tostring(srDistancePips, "#.##") + " pips).\nTests: " + str.tostring(isSupport ? testCountSupport10 : testCountResistance10) + ".\nScalp si cassure."
        else
            srAnalysis := "Aucun niveau.\nVolatilité: " + str.tostring(atrPips, "#.##") + " pips.\nProchain niveau: " + str.tostring(resistanceLevel, "#.##") + " $.\nScalp si cassure."
    else
        srAnalysis := "Repère niveaux.\nVolatilité: " + str.tostring(atrPips, "#.##") + " pips.\nProchain niveau: " + str.tostring(resistanceLevel, "#.##") + " $.\nActiver pour scalping."
    table.cell(mainTable, 0, 12, useSR and (srLongCondition or srShortCondition) ? "✅" : "❌", bgcolor=color.new(color.black, 90), text_color=color.white, text_size=size.normal)
    table.cell(mainTable, 1, 12, "S/R", bgcolor=color.new(color.black, 90), text_color=useSR ? color.green : color.red, text_size=size.normal)
    table.cell(mainTable, 2, 12, srAnalysis, bgcolor=color.new(color.black, 90), text_color=color.white, text_size=size.normal)

    string breakoutAnalysis = ""
    if useBreakout
        if bullishBreakout
            breakoutAnalysis := "Hausse.\nCassure Résistance: " + str.tostring(lastBreakoutLevel, "#.##") + " $.\nVolume: " + str.tostring(volumeRatio, "#.##") + "x moyenne.\nCible: " + str.tostring(resistanceLevel, "#.##") + " $ (+" + str.tostring((resistanceLevel - lastBreakoutLevel) / valeur_pips, "#.##") + " pips).\nScalp si continuation."
            trendScore := trendScore + 1
        else if bearishBreakout
            breakoutAnalysis := "Baisse.\nCassure Support: " + str.tostring(lastBreakoutLevel, "#.##") + " $.\nVolume: " + str.tostring(volumeRatio, "#.##") + "x moyenne.\nCible: " + str.tostring(supportLevel, "#.##") + " $ (" + str.tostring((lastBreakoutLevel - supportLevel) / valeur_pips, "#.##") + " pips).\nScalp si continuation."
            trendScore := trendScore - 1
        else
            breakoutAnalysis := "Neutre.\nPas de cassure.\nProchain niveau: " + str.tostring(resistanceLevel, "#.##") + " $.\nVolatilité: " + str.tostring(atrPips, "#.##") + " pips.\nScalp si cassure."
    else
        breakoutAnalysis := "Détecte cassures.\nVolatilité: " + str.tostring(atrPips, "#.##") + " pips.\nProchain niveau: " + str.tostring(resistanceLevel, "#.##") + " $.\nActiver pour scalping."
    table.cell(mainTable, 0, 13, useBreakout and (bullishBreakout or bearishBreakout) ? "✅" : "❌", bgcolor=color.new(color.black, 90), text_color=color.white, text_size=size.normal)
    table.cell(mainTable, 1, 13, "Breakout", bgcolor=color.new(color.black, 90), text_color=useBreakout ? color.green : color.red, text_size=size.normal)
    table.cell(mainTable, 2, 13, breakoutAnalysis, bgcolor=color.new(color.black, 90), text_color=color.white, text_size=size.normal)

    string obAnalysis = ""
    if useOB
        if lastBullishOBDetected
            obAnalysis := "Baisse.\nOB Baissier: [" + str.tostring(bottomOBBullish, "#.##") + "-" + str.tostring(topOBBullish, "#.##") + "].\nPrix: " + str.tostring(close, "#.##") + " (" + str.tostring(obDistancePips, "#.##") + " pips).\nCible: " + str.tostring(supportLevel, "#.##") + " $.\nScalp si rebond."
            trendScore := trendScore - 1
        else if lastBearishOBDetected
            obAnalysis := "Hausse.\nOB Haussier: [" + str.tostring(bottomOBBearish, "#.##") + "-" + str.tostring(topOBBearish, "#.##") + "].\nPrix: " + str.tostring(close, "#.##") + " (" + str.tostring(obDistancePips, "#.##") + " pips).\nCible: " + str.tostring(resistanceLevel, "#.##") + " $.\nScalp si rebond."
            trendScore := trendScore + 1
        else
            obAnalysis := "Aucun OB.\nVolatilité: " + str.tostring(atrPips, "#.##") + " pips.\nProchain niveau: " + str.tostring(resistanceLevel, "#.##") + " $.\nScalp si cassure."
    else
        obAnalysis := "Détecte zones OB.\nVolatilité: " + str.tostring(atrPips, "#.##") + " pips.\nProchain niveau: " + str.tostring(resistanceLevel, "#.##") + " $.\nActiver pour scalping."
    table.cell(mainTable, 0, 14, useOB and (obLongCondition or obShortCondition) ? "✅" : "❌", bgcolor=color.new(color.black, 90), text_color=color.white, text_size=size.normal)
    table.cell(mainTable, 1, 14, "OB", bgcolor=color.new(color.black, 90), text_color=useOB ? color.green : color.red, text_size=size.normal)
    table.cell(mainTable, 2, 14, obAnalysis, bgcolor=color.new(color.black, 90), text_color=color.white, text_size=size.normal)

    // Calcul du score pour la ligne "Conseil"
    float trendScoreAbs = trendScore > 0 ? trendScore : -trendScore
    float convergencePercent = activeIndicators > 0 ? (trendScoreAbs / activeIndicators) * 100 : 0
    string scoreDisplay = "Vote: " + str.tostring(trendScoreAbs, "#") + "/" + str.tostring(activeIndicators, "#") + " (" + str.tostring(convergencePercent, "#.##") + "%)"
    string scoreIcon = convergencePercent >= 75 ? "✅" : convergencePercent >= 50 ? "⚠️" : "❌"
    string advice = ""
    string signalsSummary = ""
    string suggestion = ""

    // Construction du résumé des signaux (conservé pour d'éventuelles utilisations futures)
    if useSTMA
        signalsSummary := signalsSummary + "STMA " + (stmaTrend == 1 ? "haussier (+1)" : stmaTrend == -1 ? "baissier (-1)" : "neutre (0)") + ", "
    if useRSI or useStoch
        signalsSummary := signalsSummary + "Momentum " + ((useRSI and rsiValue > 65) or (useStoch and stochK < momentumThreshold) ? "baissier (-1)" : (useRSI and rsiValue < 35) or (useStoch and stochK > momentumThreshold) ? "haussier (+1)" : (useRSI and rsiValue > momentumThreshold) or (useStoch and stochK > momentumThreshold) ? "haussier (+1)" : (useRSI and rsiValue < momentumThreshold) or (useStoch and stochK < momentumThreshold) ? "baissier (-1)" : "neutre (0)") + ", "
    if useDivergence and useRSI
        signalsSummary := signalsSummary + "Divergence " + (bullishDivergence ? "haussière (+1)" : bearishDivergence ? "baissière (-1)" : "neutre (0)") + ", "
    if useADX
        signalsSummary := signalsSummary + "ADX " + (adxBullish and adxTrending ? "haussier (+1)" : adxBearish and adxTrending ? "baissier (-1)" : "neutre (0)") + ", "
    if useMACD
        signalsSummary := signalsSummary + "MACD " + (macdCrossUp and macdLine > 0 ? "haussier (+1)" : macdCrossDown and macdLine < 0 ? "baissier (-1)" : "neutre (0)") + ", "
    if useOBV
        signalsSummary := signalsSummary + "OBV " + (obvRising ? "haussier (+1)" : obvFalling ? "baissier (-1)" : "neutre (0)") + ", "
    if useSqueeze
        signalsSummary := signalsSummary + "Squeeze " + (squeezeOff and mom > minMomentum * avgRange ? "haussier (+1)" : squeezeOff and mom < -minMomentum * avgRange ? "baissier (-1)" : "neutre (0)") + ", "
    if useCandlePatternsInConditions
        signalsSummary := signalsSummary + "Candle " + ((bullishEngulfing or hammer or invertedHammer or pinBarBullish or marubozuUp) ? "haussier (+1)" : (bearishEngulfing or shootingStar or pinBarBearish or marubozuDown) ? "baissier (-1)" : "neutre (0)") + ", "
    if useFVG
        signalsSummary := signalsSummary + "FVG " + (lastBullishFVGDetected and lastImbalanceType == "FVG" ? "haussier (+1)" : lastBearishFVGDetected and lastImbalanceType == "FVG" ? "baissier (-1)" : lastBullishFVGDetected and lastImbalanceType == "Consecutive" ? "haussier (+0.5)" : lastBearishFVGDetected and lastImbalanceType == "Consecutive" ? "baissier (-0.5)" : lastBullishFVGDetected and lastImbalanceType == "Liquidity" ? "haussier (+1.5)" : lastBearishFVGDetected and lastImbalanceType == "Liquidity" ? "baissier (-1.5)" : "neutre (0)") + ", "
    if useFibonacci
        signalsSummary := signalsSummary + "Fibonacci " + (nearFibLevelBullish and (fibClosestLevel == float(fibLevel3) or fibClosestLevel == float(fibLevel4)) ? "haussier (+1)" : nearFibLevelBearish and (fibClosestLevel == float(fibLevel3) or fibClosestLevel == float(fibLevel4)) ? "baissier (-1)" : "neutre (0)") + ", "
    if useSR
        signalsSummary := signalsSummary + "S/R " + (srLongCondition ? "haussier (+1)" : srShortCondition ? "baissier (-1)" : "neutre (0)") + ", "
    if useBreakout
        signalsSummary := signalsSummary + "Breakout " + (bullishBreakout ? "haussier (+1)" : bearishBreakout ? "baissier (-1)" : "neutre (0)") + ", "
    if useOB
        signalsSummary := signalsSummary + "OB " + (lastBullishOBDetected ? "baissier (-1)" : lastBearishOBDetected ? "haussier (+1)" : "neutre (0)") + ", "
    if signalsSummary != ""
        signalsSummary := str.substring(signalsSummary, 0, str.length(signalsSummary) - 2)

     // Suggestion d'activation d'indicateurs
    bool suggestMomentum = not (useRSI or useStoch) and (convergencePercent < 75 or trendScore == 0)
    bool suggestSqueeze = not useSqueeze and (currentATR < volatilityThreshold * atrAverage)
    bool suggestBreakout = not useBreakout and (not na(closestLevelPrice) or (currentATR > volatilityThreshold * atrAverage))
    bool suggestFVG = not useFVG and (currentATR < volatilityThreshold * atrAverage or not na(closestLevelPrice))
    bool suggestOBV = not useOBV and (volume > avgVolume * 1.5)
    suggestion := ""
    string suggestedIndicators = ""
    if activeIndicators == 0
        suggestedIndicators := "STMA et FVG pour affiner votre stratégie (tendance et zones clés)."
    else
        if suggestMomentum
            suggestedIndicators := suggestedIndicators + "RSI/Stoch (vérifier momentum), "
        if suggestSqueeze
            suggestedIndicators := suggestedIndicators + "Squeeze (anticiper volatilité), "
        if suggestBreakout
            suggestedIndicators := suggestedIndicators + "Breakout (proximité niveau), "
        if suggestFVG
            suggestedIndicators := suggestedIndicators + "FVG (zones de rebond), "
        if suggestOBV
            suggestedIndicators := suggestedIndicators + "OBV (confirmer volume), "
        if suggestedIndicators != ""
            suggestedIndicators := str.substring(suggestedIndicators, 0, str.length(suggestedIndicators) - 2)
            suggestion := "Activez pour affiner votre stratégie:\n" + suggestedIndicators + "."

    // Calcul de la prise de position pour le scalping
    float entryPrice = close
    float slPrice = na
    float tpPrice = na
    float rrRatio = na
    string positionDetails = ""

    if long_condition and convergencePercent >= 75
        // SL basé sur le niveau clé le plus proche ou volatilité
        slPrice := not na(bottomFVG) ? bottomFVG : not na(supportLevel) ? supportLevel : close - currentATR
        slPrice := math.min(slPrice, close - currentATR * 0.5) // SL serré pour scalping
        float slDistancePips = (close - slPrice) / valeur_pips
        // TP basé sur le prochain niveau clé ou volatilité (R:R 2:1)
        tpPrice := not na(topFVG) ? topFVG : not na(resistanceLevel) ? resistanceLevel : close + 2 * (close - slPrice)
        float tpDistancePips = (tpPrice - close) / valeur_pips
        rrRatio := tpDistancePips / slDistancePips
        positionDetails := "Entrée: " + str.tostring(entryPrice, "#.##") + " $.\nSL: " + str.tostring(slPrice, "#.##") + " $ (-" + str.tostring(slDistancePips, "#.##") + " pips).\nTP: " + str.tostring(tpPrice, "#.##") + " $ (+" + str.tostring(tpDistancePips, "#.##") + " pips).\nR:R: " + str.tostring(rrRatio, "#.##") + "."
    else if short_condition and convergencePercent >= 75
        slPrice := not na(topFVG) ? topFVG : not na(resistanceLevel) ? resistanceLevel : close + currentATR
        slPrice := math.max(slPrice, close + currentATR * 0.5) // SL serré pour scalping
        float slDistancePips = (slPrice - close) / valeur_pips
        tpPrice := not na(bottomFVG) ? bottomFVG : not na(supportLevel) ? supportLevel : close - 2 * (slPrice - close)
        float tpDistancePips = (close - tpPrice) / valeur_pips
        rrRatio := tpDistancePips / slDistancePips
        positionDetails := "Entrée: " + str.tostring(entryPrice, "#.##") + " $.\nSL: " + str.tostring(slPrice, "#.##") + " $ (+" + str.tostring(slDistancePips, "#.##") + " pips).\nTP: " + str.tostring(tpPrice, "#.##") + " $ (-" + str.tostring(tpDistancePips, "#.##") + " pips).\nR:R: " + str.tostring(rrRatio, "#.##") + "."

    // Construction du conseil (sans afficher signalsSummary)
    if activeIndicators == 0
        advice := "Neutre.\nAucun indicateur actif.\nVolatilité: " + str.tostring(atrPips, "#.##") + " pips.\n" + suggestion
    else if long_condition and convergencePercent >= 75
        advice := "Achat recommandé.\nScore: " + str.tostring(trendScore, "#.##") + "/" + str.tostring(activeIndicators, "#") + ".\n" + positionDetails + "\nVolatilité: " + str.tostring(atrPips, "#.##") + " pips.\n" + suggestion
    else if short_condition and convergencePercent >= 75
        advice := "Vente recommandée.\nScore: " + str.tostring(trendScore, "#.##") + "/" + str.tostring(activeIndicators, "#") + ".\n" + positionDetails + "\nVolatilité: " + str.tostring(atrPips, "#.##") + " pips.\n" + suggestion
    else if not sufficientVolatility
        advice := "Attente.\nVolatilité insuffisante (ATR: " + str.tostring(atrPips, "#.##") + " pips < Seuil: " + str.tostring(volatilityThreshold * atrAverage / valeur_pips, "#.##") + ").\n" + suggestion
    else
        advice := "Attente.\nScore: " + str.tostring(trendScore, "#.##") + "/" + str.tostring(activeIndicators, "#") + ".\nVolatilité: " + str.tostring(atrPips, "#.##") + " pips.\n" + suggestion

    table.cell(mainTable, 0, 15, activeIndicators == 0 ? "Score: 0/0 (N/A) ⚠️" : scoreDisplay + " " + scoreIcon, bgcolor=color.new(color.black, 90), text_color=color.white, text_size=size.normal)
    table.cell(mainTable, 1, 15, "Conseil", bgcolor=color.new(color.black, 90), text_color=color.white, text_size=size.normal)
    table.cell(mainTable, 2, 15, advice, bgcolor=color.new(color.black, 90), text_color=color.white, text_size=size.normal)

// TABLEAU DE BACKTESTING
if enableBacktest and (not enableBacktest or showGraphicsInBacktest)
    if not na(backtestMetricsTable)
        table.delete(backtestMetricsTable)
    backtestMetricsTable := table.new(position.bottom_right, 2, 7, border_width=1, border_color=color.white)
    table.cell(backtestMetricsTable, 0, 0, "Métriques Backtest", bgcolor=color.new(color.black, 90), text_color=color.white, text_size=size.normal)
    table.cell(backtestMetricsTable, 1, 0, "", bgcolor=color.new(color.black, 90), text_color=color.white, text_size=size.normal)
    table.cell(backtestMetricsTable, 0, 1, "Nombre de trades", bgcolor=color.new(color.black, 90), text_color=color.white, text_size=size.normal)
    table.cell(backtestMetricsTable, 1, 1, str.tostring(backtestTradeCount), bgcolor=color.new(color.black, 90), text_color=color.white, text_size=size.normal)
    table.cell(backtestMetricsTable, 0, 2, "Durée moyenne (barres)", bgcolor=color.new(color.black, 90), text_color=color.white, text_size=size.normal)
    table.cell(backtestMetricsTable, 1, 2, str.tostring(backtestAverageTradeDuration, "#.##"), bgcolor=color.new(color.black, 90), text_color=color.white, text_size=size.normal)
    table.cell(backtestMetricsTable, 0, 3, "Profit/Perte Total ($)", bgcolor=color.new(color.black, 90), text_color=color.white, text_size=size.normal)
    table.cell(backtestMetricsTable, 1, 3, str.tostring(backtestProfitLoss, "#.##"), bgcolor=backtestProfitLoss >= 0 ? color.new(color.green, 90) : color.new(color.red, 90), text_color=color.white, text_size=size.normal)
    table.cell(backtestMetricsTable, 0, 4, "Trades Gagnants", bgcolor=color.new(color.black, 90), text_color=color.white, text_size=size.normal)
    table.cell(backtestMetricsTable, 1, 4, str.tostring(winningTrades), bgcolor=color.new(color.green, 90), text_color=color.white, text_size=size.normal)
    table.cell(backtestMetricsTable, 0, 5, "Trades Perdants", bgcolor=color.new(color.black, 90), text_color=color.white, text_size=size.normal)
    table.cell(backtestMetricsTable, 1, 5, str.tostring(losingTrades), bgcolor=color.new(color.red, 90), text_color=color.white, text_size=size.normal)
    table.cell(backtestMetricsTable, 0, 6, "Max Drawdown ($)", bgcolor=color.new(color.black, 90), text_color=color.white, text_size=size.normal)
    table.cell(backtestMetricsTable, 1, 6, str.tostring(maxDrawdown, "#.##"), bgcolor=color.new(color.red, 90), text_color=color.white, text_size=size.normal)
