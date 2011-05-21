<?

// must have "enable_dl = On" in php.ini
if(!(bool)ini_get( "enable_dl" ) || (bool)ini_get( "safe_mode" )) { 
  die( "Loading extensions not allowed\n" ); 
} 
// will also need to do something like:
//   sudo ln -s $PWD/coopy.so /usr/lib/php5/20090626/coopy.so

include("coopy.php");

function convertFile($src,$dest) { 
  $book = new PolyBook();
  $book->read($src);
  $book->write($dest);
}

function generateDiff($version1,$version2,$dest) {
  $book1 = new PolyBook();
  $book1->read($version1);
  $book2 = new PolyBook();
  $book2->read($version2);
  $out = new CompareOutput($dest);
  
  $patcher = Patcher_createByName('tdiff');
  $flags = new CompareFlags();
  $flags->setOutput($out);
  $cmp = new BookCompare();
  $cmp->compare($book1,$book1,$book2,$patcher,$flags);
}

class TableDiff extends Patcher {

  function changeRow($change0) {
    $change = new RowChange($change0);
    printf("Row change of type '%s'\n", $change->modeString());
    $names = new StringList(RowChange_names_get($change0));
    $cond = new CellMap(RowChange_cond_get($change0));
    $val = new CellMap(RowChange_val_get($change0));
    for ($i=0; $i<$names->size(); $i++) {
      $c = $names->get($i);
      if ($cond->has_key($c)) {
	printf("  condition: %s = %s\n", $c, $cond->get($c)->toString());
      }
    }
    for ($i=0; $i<$names->size(); $i++) {
      $c = $names->get($i);
      if ($val->has_key($c)) {
	printf("  value: %s -> %s\n", $c, $val->get($c)->toString());
      }
    }
  }
}

function examineDiff($version1,$version2) {
  $book1 = new PolyBook();
  $book1->read($version1);
  $book2 = new PolyBook();
  $book2->read($version2);
  $cmp = new BookCompare();
  $cmp->compare($book1,$book1,$book2,new TableDiff(),new CompareFlags());
}

generateDiff("named_numbers.csv","named_numbers_three_23.csv","patch.diff");
convertFile("named_numbers.csv","named_numbers.sqlite");
examineDiff("named_numbers.csv","named_numbers_three_23.csv");

?>
