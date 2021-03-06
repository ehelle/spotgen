function getTiffInfo($tif) {
    $dx = 0
    $dy = 0
    $ulx = 0
    $ulx = 0
    gdalinfo $tif | `
    % { 
        if ($_ -match "Pixel Size = \((-?\d+\.\d+),(-?\d+\.\d+)\)") {
            $dx = [double]$($matches[1])
            $dy = [double]$($matches[2])
        }
        if ($_ -match "Upper Left\s+\(\s*(-?\d+\.\d+),\s*(-?\d+\.\d+)\)") {
            $ulx = [double]$($matches[1])
            $uly = [double]$($matches[2])
        }
      }
    return ($dx, $dy, $ulx, $uly)
}


function image2geoCoord($dx, $dy, $ulx, $uly, $x, $y) {
    $geoX = ([double]$x + 0.5) * $dx + $ulx
    $geoY = ([double]$y + 0.5) * $dy + $uly
    return ($geoX, $geoY)
}


function IJresult2geocsv($infile, $outfile, $tif) {
    ($dx, $dy, $ulx, $uly) = getTiffInfo $tif
    $tsv = Import-Csv -path $infile -Delimiter "`t" -header "N","X","Y"
    echo "x,y,height" | out-file $outfile -encoding ascii
    $tsv | select -skip 1 | `
    % {
        ($x,$y) = image2geoCoord $dx $dy $ulx $uly $($_.X) $($_.Y)
        [int]$height = gdallocationinfo $tif $($_.X) $($_.Y) -valonly
        echo $("$x,$y,$height") | out-file $outfile -encoding ascii -append
      }
}

function getSpotHeightsFromTiff($tifdir, $shpdir, $tmpdir) {
    gci $tifdir -filter *.tif |  `
    % {
        $shpfile = $("$shpdir\$($_.basename).shp")
        if (-not (Test-Path $shpfile)) { 
        ($dx, $dy, $ulx, $uly) = getTiffInfo $_.fullname
        $IJres = $("$tmpdir\$($_.basename).txt")
        $ijmFile = $("$tmpdir\$($_.basename).ijm")
        echo $("open(`"$($_.fullname)`");").replace("\","\\") | out-file $ijmFile -encoding ascii
        echo "run(`"Find Maxima...`", `"noise=50 output=List exclude`");" | out-file $ijmFile -append -encoding ascii
        #echo "IJ.redirectErrorMessages()" | out-file $ijmFile -append -encoding ascii
        echo $("saveAs(`"Results`", `"$($IJres.replace("\","\\"))`");") | out-file $ijmFile -append -encoding ascii
        #echo "IJ.redirectErrorMessages()" | out-file $ijmFile -append -encoding ascii
        #echo "run(`"Close All`")" | out-file $ijmFile -append -encoding ascii
        echo "run(`"Quit`");)" | out-file $ijmFile -append -encoding ascii
        imagej -macro $ijmFile | Out-Null
        IJresult2geocsv $IJres $("$tmpdir\$($_.basename).csv") $_.fullname
        ogr2ogr -f "ESRI shapefile" $shpfile $("$tmpdir\$($_.basename).csv") `
                -oo X_POSSIBLE_NAMES=x -oo Y_POSSIBLE_NAMES=y -oo KEEP_GEOM_COLUMNS=NO -a_srs "epsg:4326"
        rm $("$tmpdir\$($_.basename)*")
        }
      }
}