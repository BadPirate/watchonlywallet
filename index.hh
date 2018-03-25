<?hh
  require_once('vendor/hh_autoload.php');

  $btc = isset($_GET['btc']) ? explode(',',$_GET['btc']) : [];
  $eth = isset($_GET['eth']) ? explode(',',$_GET['eth']) : [];

  $addresses =
  <div class="panel">
    <p>Welcome to Watcher Online Wallet.  A light-weight, multi-currency <b>Watch Only</b> wallet.</p>
    <p>Add your watch only addresses below to see how much your coins are worth, and bookmark the URL for quick access in the future</p>
  </div>;

  if (count($btc) > 0)
  {
    $addresses = <div class="list-group"/>;
    $btctotal = 0;
    foreach ($btc as $address) {
      $value = file_get_contents("https://blockchain.info/q/addressbalance/$address")/100000000;
      $btctotal = $value + $btctotal;
      $deleteAction = "delete_coin_address('btc','$address');";
      $addresses->appendChild(
        <div class="list-group-item">
          <i class="fa fa-trash pull-right" onclick={$deleteAction}/>
          <span class="badge">
            {$value}<i class="fa fa-btc"></i>
          </span>
          <a href={"https://blockchain.info/address/$address"} target="new">
            {$address}
          </a>
        </div>);
    }
  }

  $total = <div/>;
  if ($btctotal > 0)
  {
    setlocale(LC_MONETARY, 'en_US');
    $btcRate = 1/file_get_contents("https://blockchain.info/tobtc?currency=USD&value=1");
    $btcUSDTotal = money_format('%.2n',$btctotal * $btcRate);
    $total =
    <div class="panel">
      {$btctotal}<i class="fa fa-btc"/> @ {$btcRate}/USD = {$btcUSDTotal}<br/>
    </div>;
  }

  function addCoin($coinType = "btc",) {
    $divID = "$coinType-address-add";
    $onclick = "add_coin_address('$coinType', $( '#$divID' ).val());";
    $placeholder = strtoupper($coinType)." Address";
    return
    <div class="input-group">
      <input type="text" placeholder={$placeholder} class="form-control" id={$divID}/>
      <span class="input-group-btn">
        <button class="btn btn-default" type="button" onclick={$onclick}>Add</button>
      </span>
    </div>;
  }

  $addAddress = <div class="panel"/>;
  $addAddress->appendChild(addCoin("btc"));
  // $addAddress->appendChild(addCoin("eth"));

  $otherCoins =
    <div class="panel">
      <p>I built this site to track the total values of the various crypto currencies I hold.
        As such, it currently only supports those coins that I have some value in.  If you'd like to see
        another type of crypto currency supported, the easiest way would be to put about $10 in a wallet of that
        type, and send the private key to <a href="mailto:request@watchonlywallet.net">request@watchonlywallet.net</a>
        along with information about the blockchain for that wallet as known, and if I add it I'll transfer the coins :)</p>
    </div>;

  $js_addresses = "{ 'btc' : ".json_encode($btc).", 'eth' : ".json_encode($eth)."}";
  $template =
  <html lang="en">
    <head>
      <link rel="stylesheet" href="https://cdnjs.cloudflare.com/ajax/libs/font-awesome/4.7.0/css/font-awesome.min.css"/>
      <script src="//code.jquery.com/jquery-1.11.0.min.js"></script>
      <meta name="viewport" content="width=device-width, initial-scale=1"/>
      <!-- Latest compiled and minified CSS -->
      <link
        rel="stylesheet"
        href="//maxcdn.bootstrapcdn.com/bootstrap/3.2.0/css/bootstrap.min.css"
      />
      <script src="//code.jquery.com/jquery-1.11.0.min.js"></script>
      <!-- Latest compiled and minified JavaScript -->
      <script
        src="//maxcdn.bootstrapcdn.com/bootstrap/3.2.0/js/bootstrap.min.js"
      />
      <script type="text/javascript">
        var crypto_addresses = {$js_addresses};
      </script>
      <script src="wow.js"></script>
      <title>Watcher Online Wallet</title>
    </head>
    <body>
    {$addresses}
    {$total}
    {$addAddress}
    {$otherCoins}
    </body>
  </html>;

  print $template;
