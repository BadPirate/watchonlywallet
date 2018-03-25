<?hh
  require_once('vendor/hh_autoload.php');

  $btc = isset($_GET['btc']) ? explode(',',$_GET['btc']) : [];

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
      $deleteAction = "deleteBitcoin('$address');";
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

  setlocale(LC_MONETARY, 'en_US');
  $btcRate = 1/file_get_contents("https://blockchain.info/tobtc?currency=USD&value=1");
  $btcUSDTotal = money_format('%.2n',$btctotal * $btcRate);
  $total =
  <div class="panel">
    {$btctotal}<i class="fa fa-btc"/> @ {$btcRate}/USD = {$btcUSDTotal}<br/>
  </div>;

  $addAddress =
  <div class="input-group">
    <input type="text" placeholder="Bitcoin Address" class="form-control" id="bitcoin-address-add"/>
    <span class="input-group-btn">
      <button class="btn btn-default" type="button" onclick="addBitcoin($( '#bitcoin-address-add' ).val());">Add</button>
    </span>
  </div>;

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
        var bitcoin_addresses = {json_encode($btc)};
      </script>
      <script src="wow.js"></script>
      <title>Watcher Online Wallet</title>
    </head>
    <body>
    {$addresses}
    {$total}
    {$addAddress}
    </body>
  </html>;

  print $template;
