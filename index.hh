<?hh
  require_once('vendor/hh_autoload.php');
  require_once('keys.hh');

  $btc = isset($_GET['btc']) ? explode(',',$_GET['btc']) : [];
  $eth = isset($_GET['eth']) ? explode(',',$_GET['eth']) : [];
  $ltc = isset($_GET['ltc']) ? explode(',',$_GET['ltc']) : [];
  $xmr = isset($_GET['xmr']) ? explode(',',$_GET['xmr']) : [];

  $addresses =
  <div class="panel panel-default">
    <div class="panel-heading">
      Welcome to Watcher Online Wallet
    </div>
    <div class="panel-body">
      A light-weight, multi-currency <b>Watch Only</b> wallet. Add your watch only addresses below
      to see how much your coins are worth, and bookmark the URL for quick access in the future
    </div>
  </div>;

  function value_address($coin, $address)
  {
    switch ($coin) {
      case 'btc':
        return file_get_contents("https://blockchain.info/q/addressbalance/$address")/100000000;
      case 'eth':
        $url = "https://api.etherscan.io/api?module=account&action=balance&address=$address&tag=latest&apikey=".ETHERSCAN_API_KEY;
        $json = json_decode(file_get_contents($url));
        return ($json->result)/(pow(10,18));
      case 'ltc':
        $url = "https://api.blockcypher.com/v1/ltc/main/addrs/$address/balance";
        $json = json_decode(file_get_contents($url));
        return ($json->balance)/pow(10,8);
      case 'xmr':
        return $address; // Balance only
    }
    return 0;
  }

  function link_address($coin, $address)
  {
    switch ($coin) {
      case 'btc':
        return "https://blockchain.info/address/$address";
      case 'eth':
        return "https://etherscan.io/address/$address";
      case 'ltc':
        return "https://live.blockcypher.com/ltc/address/$address/";
      case 'xmr':
        return "https://monero.stackexchange.com/a/4227/4700";
    }
    return "";
  }

  function coin_fa($coin)
  {
    switch ($coin) {
      case 'btc': return 'fab fa-btc';
      case 'eth': return 'fab fa-ethereum';
      case 'ltc': return 'fas fa-lightbulb';
      case 'xmr': return 'fab fa-monero';
    }
    return '';
  }

  $rates = json_decode(file_get_contents("https://min-api.cryptocompare.com/data/price?fsym=USD&tsyms=ETH,BTC,LTC,XMR"),true);
  setlocale(LC_MONETARY, 'en_US');
  function coin_total($coin,$total,$rates,&$totalusd)
  {
    if ($total == 0) return <div/>;
    $rate = 1/$rates[strtoupper($coin)];
    $usd = money_format('%.2n',$total * $rate);
    $totalusd = $totalusd + $usd;
    return
      <div class="panel">
        {$total}<i class={coin_fa($coin)}/> @ {$rate}/USD = {$usd}<br/>
      </div>;
  }

  function value_addresses($coin, $addresses, &$total, &$totalusd)
  {
    global $rates;
    if (count($addresses) == 0)
    {
      return <div/>;
    }

    $result = <div class="list-group"/>;
    foreach ($addresses as $address) {
      $value = value_address($coin, $address);
      $total = $total + $value;
      $coin_class = coin_fa($coin);
      $link = link_address($coin, $address);
      $deleteAction = "delete_coin_address('$coin','$address');";
      $result->appendChild(
        <div class="list-group-item">
          <i class="fa fa-trash pull-right" onclick={$deleteAction}/>
          <span class="badge">
            {$value}<i class={$coin_class}/>
          </span>
          <a href={$link} target="new">
            {$address}
          </a>
        </div>);
    }
    return
      <div>
        {$result}
        {coin_total($coin,$total,$rates,$totalusd)}
      </div>;
  }

  $total = <div/>;
  $totalsection = <div/>;
  $totalusd = 0;

  if (count($btc) > 0 || count($eth) > 0 || count($ltc) > 0 || count($xmr) > 0)
  {
    $coin_container = <div class="panel-body"/>;
    $btctotal = 0;
    $coin_container->appendChild(value_addresses('btc',$btc,$btctotal,$totalusd));
    $ethtotal = 0;
    $coin_container->appendChild(value_addresses('eth',$eth,$ethtotal,$totalusd));
    $ltctotal = 0;
    $coin_container->appendChild(value_addresses('ltc',$ltc,$ltctotal,$totalusd));
    $xmrtotal = 0;
    $coin_container->appendChild(value_addresses('xmr',$xmr,$xmrtotal,$totalusd));

    $addresses =
      <div class="panel panel-default">
        <div class="panel-heading">
          Watch Only Wallet
        </div>
        {$coin_container}
      </div>;

    if ($totalusd > 0)
    {
      $totalformat = money_format('%.2n',$totalusd);
      $totalsection =
        <div class="panel panel-primary">
          <div class="panel-heading">
            Total USD: {$totalformat}
          </div>
        </div>;
    }
  }

  function addCoin($coinType = "btc",) {
    $divID = "$coinType-address-add";
    $onclick = "add_coin_address('$coinType', $( '#$divID' ).val());";
    $placeholder = ($coinType == 'xmr') ? "XMR Coin Total" : strtoupper($coinType)." Address";
    return
    <div class="input-group">
      <input type="text" placeholder={$placeholder} class="form-control" id={$divID}/>
      <span class="input-group-btn">
        <button class="btn btn-default" type="button" onclick={$onclick}>Add</button>
      </span>
    </div>;
  }

  $add_by_address_body = <div class="panel-body"/>;
  $add_by_address_body->appendChild(addCoin("btc"));
  $add_by_address_body->appendChild(addCoin("eth"));
  $add_by_address_body->appendChild(addCoin('ltc'));

  $add_by_value_body = <div class="panel-body"/>;
  $add_by_value_body->appendChild(addCoin('xmr'));

  $addAddress =
    <div>
      <div class="panel panel-default">
        <div class="panel-heading">Add currency by address</div>
        {$add_by_address_body}
      </div>
      <div class="panel panel-default">
        <div class="panel-heading">Add currency by amount you own.</div>
        {$add_by_value_body}
      </div>
    </div>;

  $otherCoins =
    <div class="panel panel-default">
      <div class="panel-heading">
        Don't see your favorite here?
      </div>
      <div class="panel-body">
        I built this site to track the total values of the various crypto currencies I hold.
        As such, it currently only supports those coins that I have some value in.  If you'd like to see
        another type of crypto currency supported, the easiest way would be to put about $10 in a wallet of that
        type, and send the private key to <a href="mailto:request@watchonlywallet.net">request@watchonlywallet.net</a>
        along with information about the blockchain for that wallet as known, and if I add it I'll transfer the coins :)
      </div>
    </div>;

  $js_addresses = "{ 'btc' : ".json_encode($btc).",
                     'eth' : ".json_encode($eth).",
                     'ltc' : ".json_encode($ltc).",
                     'xmr' : ".json_encode($xmr)." }";
  $template =
  <html lang="en">
    <head>
      <link rel="apple-touch-icon" sizes="57x57" href="images/Slice57w.png" />
      <link rel="apple-touch-icon" sizes="72x72" href="images/Slice72w.png" />
      <link rel="apple-touch-icon" sizes="114x114" href="images/Slice114w.png" />
      <link rel="apple-touch-icon" sizes="144x144" href="images/Slice144w.png" />
      <script defer="" src="https://use.fontawesome.com/releases/v5.0.6/js/all.js"></script>
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
    {$totalsection}
    {$addresses}
    {$total}
    {$addAddress}
    {$otherCoins}
    </body>
  </html>;

  print $template;
